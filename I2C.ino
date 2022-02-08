//$1 14 0 0 0 1 0 0 90 101 1 208;

int inArray[1];
int outArray[5];

unsigned long sessionTimer = 0;

void init_I2C(){
  Wire.begin(0x01); // инициализируем устройсво как slave
  Wire.onRequest(write_I2C_slave);
  Wire.onReceive(read_I2C_slave);
}

void write_I2C_slave(){
  const int lenOutArray = sizeof(outArray) / sizeof(outArray[0]);
  const int lenWriteArray = (lenOutArray * 2) + 4;
  byte writeArray[lenWriteArray];
  long acc = 0;
  

  for(int i = 0; i < lenOutArray; i++ ){
    writeArray[i * 2] = (byte)outArray[i];
    writeArray[(i * 2) + 1 ] = (byte)(outArray[i] >> 8);
    acc += outArray[i];
  }

  for(int i = 0; i < 4; i++){
    writeArray[i +(lenOutArray * 2)] = i == 0 ? (byte) acc : (byte) (acc >> (i * 8));
  }
  
  Wire.write(writeArray, lenWriteArray);
}



bool read_I2C_slave(){
  const int lenInArray = sizeof(inArray ) / sizeof(inArray[0]);
  int bufferInArray[lenInArray];
  long crc = 0;
  long acc = 0;
  
  while(int numBytes = Wire.available()){
    byte secondBuffer[numBytes];
    for(int i = 0; i < numBytes; i++){
      secondBuffer[i] = Wire.read();
    }

    for(int i = 0; i < lenInArray; i++){
        bufferInArray[i] = (((int)secondBuffer[(i * 2) + 1 ]) << 8 ) | secondBuffer[ i * 2];
        acc += bufferInArray[i];
    }
    
    for(int i = 0; i < 4; i++){
      crc = i == 0 ? secondBuffer[i + (lenInArray * 2)] : (((int)secondBuffer[i + (lenInArray * 2)]) << (8 * i)) | crc;
    } 

    if(acc == crc){
      for(int i = 0; i < lenInArray; i++){
        uploadSessionTimer();
        inArray[i] = bufferInArray[i];
        uart.print(bufferInArray[i]);
        uart.print(" ");
      }
      uart.println("CRC: " + crc);
      
    }else{
      uart.print("Ошибка crc: ");
      uart.println(crc);
      
    }
  }   
}

void i2cArrayInUpdater(){
  int needThemp = inArray[0];
  if(needThemp > 80){
    needThemp = 80;
  };
}

void i2cSesionWtacher(int timer){
  if((millis() - sessionTimer) > timer){
    sessionTimer = millis();
    espReset(4);
  }
}

void uploadSessionTimer(){
  sessionTimer = millis();
}

void espReset(int pin){
  digitalWrite(pin, LOW);
  delay(500);
  digitalWrite(pin, HIGH); 
}
