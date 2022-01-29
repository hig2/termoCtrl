

void termostat(){
  const int gistTemp = 5;
  static unsigned long timerDef = 0;

  if(needThemp > 80){
    needThemp = 80;
  };


  if(themp > needThemp){
    buferComand[0] = 0; //останавливаем горелку

  }else if(themp < (needThemp - gistTemp)){ 
    if(themp != 0){
        timerDef = millis();
        buferComand[0] = 1; // росжиг
    }else if((millis() - timerDef) > 300000){
      buferComand[0] = 27; // ошибка
    }
  }  
}
