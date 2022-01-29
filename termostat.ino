const int termoPin = A0;

const unsigned int temperature_table[] {
    63358, 63250, 63137, 63020, 62899, 62772, 62641, 62504,
    62363, 62216, 62064, 61906, 61743, 61574, 61399, 61218,
    61031, 60837, 60637, 60431, 60218, 59997, 59768, 59533,
    59291, 59041, 58777, 58506, 58227, 57940, 57645, 57349,
    57047, 56736, 56419, 56094, 55760, 55419, 55070, 54715,
    54352, 53980, 53601, 53214, 52821, 52421, 52012, 51597,
    51176, 50748, 50313, 49869, 49418, 48961, 48499, 48031,
    47558, 47079, 46596, 46108, 45616, 45120, 44621, 44118,
    43612, 43103, 42591, 42076, 41560, 41041, 40521, 39999,
    39476, 38952, 38428, 37904, 37379, 36855, 36331, 35809,
    35288, 34764, 34242, 33722, 33205, 32690, 32176, 31666,
    31158, 30654, 30154, 29664, 29178, 28696, 28219, 27746,
    27277, 26813, 26354, 25900, 25451, 24998, 24551, 24110,
    23674, 23244, 22818, 22397, 21983, 21575, 21173, 20775,
    20384, 19999, 19620, 19248, 18879, 18517, 18161, 17811,
    17467, 17124, 16788, 16457, 16132, 15814
};


void termostat(){
  const int gistTemp = 10;
  static unsigned long timerDef = 0;

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



void readTermo(int timer){
  static unsigned long t = 0;
  if(millis() - t > timer){
    themp = getAdcTermo();
     //uart.println(themp);
    t = millis();
  }
}

byte getAdcTermo(){
  unsigned int acc = 0;
  for(int i = 0; i < 64; i++){
    acc += analogReadFast(termoPin); // ускоренная версия аналог рида 21мКс вместо 112мКс
  }
  //uart.println(acc);
  if(acc >= temperature_table[0]){ 
    return 0;
  }else if(acc <= temperature_table[125]){ // при двух преведущих операциях мы сравниваем акк на прямую, а в последущем через выччитание ?
    return 125;
  }else {// в описании к функции abs() предупреждается о некоректной работе функции при внесении в качестве аргумента мат.вычисления (нам нужно математику вынести за abs())
    int themp = abs(acc - temperature_table[0]); 
    int item = 0;
    for(int i = 1; i < 125; i++){
      if(abs(acc - temperature_table[i])< themp){
        themp = abs(acc - temperature_table[i]);
        item = i;
      }
    }
    return item;
  }
}
