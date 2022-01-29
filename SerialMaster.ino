 

/*
 * // $1 14 0 0 0 1 0 0 90 101 1 208;

int globalState[12]={ // необходим для мониторинга 
  0, //[0] Общее состояние : 0 - ожидание, 1 - росжиг, 2 - горение 
  0, //[1] Текущая температура топлива
  0, //[2] Статус уровня топлива депульсатора (0,1)
  0, //[3] Статус уровня топлива буферного бака (0,1)
  0, //[4] Статус горения (0,1)
  0, //[5] Показания потребляймого тока внутреннего двигателя 
  0, //[6] Пневма реле открыто
  0, //[7] Номер ошибки(1 - не удалось зажечь,2 - не удалось заполнить бак топливом, 3 - перегрев топлива, 4 - блокировка, 5 - клин двигателя  )
  0, //[8] Команда на выполнение (Удерживаймая температура)
  0, //[9] Версия прошивки
  0, //[10] ID
  0  //[11] CRC(контрольная сумма пакета)
};

*/

int buferComand[3]={
  0, // команда на переход в состояние
  85, // поддерживаемая температура
  0  // crc
};

void serialWatcher(int timer){
  static unsigned long t = 0;
  if(millis() - t > timer){
    serialMaster();
    t = millis();
  }
}

void serialMaster(){
  static int lengthBuferComand = sizeof(buferComand) / sizeof(buferComand[0]);
  String acc = "$";
  unsigned long crc = 0;
  int lengthGlobalStateBufer = 12;
  int globalStateBufer[lengthGlobalStateBufer];
   if(parsePacket((int*)globalStateBufer)){
    for(byte i = 0; i < lengthGlobalStateBufer - 1; i++){ // расчет CRC
      crc += globalStateBufer[i];
    }

    if(globalStateBufer[lengthGlobalStateBufer - 1] == crc){ //проверка контрольной суммы пройдена
      //записываем результаты в необходимые перменные

      outArray[0] = globalStateBufer[7]; // ошибка
      outArray[1] = globalStateBufer[0]; // статус
      outArray[2] = globalStateBufer[1]; // температура топлива
      outArray[3] = themp;
      outArray[4] = needThemp;
      
      
     //сразу же формируем ответ на отправку
     crc = 0; //очищаем преведущую CRC
     for(byte i = 0; i < lengthBuferComand; i++){ // расчет CRC
        crc += buferComand[i];
     }
     for(byte i = 0; i < lengthBuferComand; i++){ // формируем посылку 
      if(lengthBuferComand - 1 == i){
          acc+= crc;
          acc+= ";";
        }else{
          acc+= buferComand[i];
          acc+= " ";
        }  
      }
      uart.println(acc); // отпровляем посылку
    }else{
      //обработка ошибки
    } 
  }
}

boolean parsePacket(int *intArray) {
    if (uart.available()) {
        uint32_t timeoutTime = millis();
        int value = 0;
        byte index = 0;
        boolean parseStart = 0;

        while (millis() - timeoutTime < 100) {
            if (uart.available()) {
                timeoutTime = millis();
                if (uart.peek() == '$') {
                    parseStart = true;
                    uart.read();
                    continue;
                }
                if (parseStart) {
                    if (uart.peek() == ' ') {
                        intArray[index] = value / 10;
                        value = 0;
                        index++;
                        uart.read();
                        continue;
                    }
                    if (uart.peek() == ';') {
                        intArray[index] = value / 10;
                        uart.read();
                        return true;
                    }
                    value += uart.read() - '0';
                    value *= 10;
                }
                else
                {
                    uart.clear(); //возможно не будет работать нужна очистка 
                }
            }
        }
    }
    return false;
}
