 
//$1 7 0 0 0 1 0 0 85 102 1 197;
#include "avdweb_AnalogReadFast.h" 
#include <Wire.h>
#include "GyverWDT.h"
int needThemp = 0;
int themp = 0;



void setup(){
  uart.begin(19200);
   pinMode(4, OUTPUT);//reset for esp
   digitalWrite(4, HIGH); 
  init_I2C();
  Watchdog.enable(RESET_MODE, WDT_PRESCALER_1024); 
}

void loop(){
  readTermo(100);
  termostat();
  serialMaster();
  i2cSesionWtacher(5000);
  i2cArrayInUpdater();
  Watchdog.reset();
}
