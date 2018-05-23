#define keyborardspeedo 200
#define  UP  'w'
#define  DOWN  's'
#define  LEFT  'a'
#define   RIGHT  'd'
#define SPEED_UP 'j'
#define SPEED_DOWN 'k'

unsigned long keyboardTime;

void Mode3()
{

  keyboardTime = millis();
  Serial.println("MODE2 START");
  while (1)
  {
    //接地时暂停
    if (digitalRead(switchPin) == 1)
    {
      Serial.println("MODE2 STOP");
      while (digitalRead(5) == 1);
    }

    if ( keyboardTime - millis() >= keyborardspeedo)
    {
      getAccelerometerData();
      getGyroscopeData();
      getInclination;

      if (Awz[0] <= -40)
        Keyboard.press( UP );
      else if (Awz[0] >= 40)
        Keyboard.press( DOWN );

      if (Awz[1] >= 40)
        Keyboard.press( RIGHT );
      else if (Awz[1] <= -40)
        Keyboard.press( LEFT );


      if ( millis() - wheelTime >= wheelSpeedo )
      {
        get_touchButton_State();        //得到按键状态
        if ( touchButton_State[1] )
        {
          Keyboard.press( SPEED_UP );
          moveTime = millis();
        }
        else if ( touchButton_State[0] )
        {
          Keyboard.press( SPEED_DOWN );
          moveTime = millis();
        }
      }
      
      Keyboard.releaseAll();
    }
    //BlueTeeth
    getBlueTeeth();
    if (MODE != 3)
      return;    

      
  }
}

