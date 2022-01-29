
//$1 7 0 0 0 1 0 0 85 102 1 197;
#include "avdweb_AnalogReadFast.h" 
#include <Wire.h>
#include "GyverWDT.h"
int needThemp = 55;
int themp = 0;



void setup(){
  uart.begin(19200);
  init_I2C();
  Watchdog.enable(RESET_MODE, WDT_PRESCALER_1024); 
}

void loop(){
  readTermo(100);
  termostat();
  serialMaster();
  Watchdog.reset();
}
