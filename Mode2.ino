#define clickSpeedo 200  //点击时间间隔
#define  mouseSpeedo  50
#define jumpSpeedo 1500
#define Xspeed  6
#define Yspeed  4

//定义游戏动作键
#define  UP  'w' //前进
#define  DOWN  's' //后退
#define  LEFT  'a' //左
#define   RIGHT  'd' //右
#define JUMP ' '  //跳跃
#define ROLL 'b'  //滚动

unsigned long moveTime = 0,
              jumpTime = 0;
int jumpDelay = 0; //起跳后一秒内没有上下切换
void Mode2()
{
  MouseTime = millis();
  clickTime = moveTime = jumpTime = millis();

  Serial.println("MODE2 START");
  while (1)
  {
    //接地时暂停


    byte wheel = 0, mouseX = 0 , mouseY = 0;


    //检测手指实现左右键点击
    if ( millis() - clickTime >= clickSpeedo)
    {
      getFinger_State();            //得到手指状态
      if (Finger_State[1] > 1) //鼠标左键
      {
        if (!Mouse.isPressed(MOUSE_LEFT))
        {
          Mouse.press(MOUSE_LEFT);
          continue;
        }
      }
      else Mouse.release(MOUSE_LEFT);


      if (Finger_State[0] > 1) //鼠标右键
      {
        if (!Mouse.isPressed(MOUSE_RIGHT))
        {
          Mouse.press(MOUSE_RIGHT);
          continue;
        }
      }
      else
        Mouse.release(MOUSE_RIGHT);
      clickTime = millis();
    }


    //有些游戏如丧尸围城用现在的方法走路一瘸一拐的，推测是按键不连续导致的，但是大部分游戏没有问题
    get_touchButton_State();        //得到按键状态
    if ( touchButton_State[1] )
    {
      Keyboard.press( UP );
      moveTime = millis();
    }
    else if ( touchButton_State[0] )
    {
      Keyboard.press( DOWN );
      moveTime = millis();
    }
    else  Keyboard.releaseAll();

    if( isJump())
    {
      Keyboard.press( JUMP );
      Keyboard.releaseAll();     
    }

    if( isRoll() )
    {
      Keyboard.press( ROLL );
      Keyboard.releaseAll();        
    }
    
    if ( micros() > 4294967295 - 10000 ) { //防止micros()溢出
      MouseTime = micros();
      clickTime = millis();
      wheelTime = millis();
    }



    if ( MouseTime - millis() >= mouseSpeedo)
    {
      getAccelerometerData();     //得到ADXL345数据
      getGyroscopeData();         //得到陀螺仪数据
      getInclination();

      //在10度内才会移动
      if (  fabs(Awz[0]) <= 70)
      {
        mouseX =  - gyro[2] / Xspeed ;
        mouseY = gyro[1] / Yspeed;
      }

      MouseTime = millis();
    }

    //----------最后是控制鼠标的函数---------//
    Mouse.move(mouseX, mouseY , wheel );

    //BlueTeeth
    getBlueTeeth();
    if (MODE != 2)
      return;

    if (digitalRead(switchPin) == 1)
    {
      Serial.println("MODE2 STOP");
      while (digitalRead(5) == 1);
    }

  }
}

