#define clickSpeedo 200  //点击时间间隔
#define  wheelSpeedo  90
#define  mouseSpeedo  50
#define Xspeed  3
#define Yspeed  3


unsigned long wheelTime = 0;
void Mode1()
{
  MouseTime = millis();
  clickTime = wheelTime = millis();
  Serial.println("MODE1 START");

  int LastClickPositionX = 0,
      LastClickPositionY = 0; //定义上一次点击相对于这一次的位置，用于防止双击时的抖动
  unsigned long LastClickTime = 0;
  
  while (1)
  {
    //接地时暂停
    if (digitalRead(switchPin) == 1)
    {
      Serial.println("MODE1 STOP");
      while (digitalRead(5) == 1);
    }


    byte wheel = 0, mouseX = 0 , mouseY = 0;

    //检测手指实现左右键点击
    if ( millis() - clickTime >= clickSpeedo)
    {
      getFinger_State();            //得到手指状态
      if (Finger_State[1] > 1) //鼠标左键
      {
        if (!Mouse.isPressed(MOUSE_LEFT))
        {
          if( millis() - LastClickTime <= 400)
          {
            if( LastClickPositionX*LastClickPositionX +LastClickPositionY*LastClickPositionY <= 2500 )
              Mouse.move( -LastClickPositionX, -LastClickPositionY , 0 ); //判断是双击，且距离不远，鼠标移回原来的位置
          }

          //记录点击时的初始位置
          LastClickPositionX = 0;
          LastClickPositionY = 0;
          LastClickTime = millis();
          
          Mouse.press(MOUSE_LEFT);
          delay(5);
          continue;
        }
      }
      else Mouse.release(MOUSE_LEFT);

      if (Finger_State[0] > 1) //鼠标右键
      {
        if (!Mouse.isPressed(MOUSE_RIGHT))
        {
          Mouse.press(MOUSE_RIGHT);
          vibrate();
          delay(5);
          continue;
        }
      }
      else{
        Mouse.release(MOUSE_RIGHT);
        vibrate();  //点击成功，马达震动提示
      }
      clickTime = millis();
    }


    //检测滚轮
    if ( millis() - wheelTime >= wheelSpeedo )
    {
      get_touchButton_State();        //得到按键状态
      if ( touchButton_State[1] )
      {
        Mouse.move(0, 0, 1);
        wheelTime = millis();
        continue;
      }
      else if ( touchButton_State[0] )
      {
        Mouse.move(0, 0, -1);
        wheelTime = millis();
        continue;
      }
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

      //在俯仰角10度内才会移动
      if (  fabs(Awz[0]) <= 13)
      {
        mouseX =  - gyro[2] / Xspeed ;
        
        if(getLigterSensor( lightSensorPin ) )  //判断是否是在平面上运动
           mouseY = gyro[1] * 16 / Yspeed / (16 + fabs(Awz[0]) ); //设置由手掌摆动带来的减速惩罚，减少手掌抖动时的影响
        else 
          mouseY = gyro[0] / Yspeed;   //切换到平面模式
      }

      MouseTime = millis();
    }

    //每次叠加记录相对位置
    LastClickPositionX += mouseX;
    LastClickPositionY += mouseY;
    
    //----------最后是控制鼠标的函数---------//
    Mouse.move(mouseX, mouseY , wheel );


    //BlueTeeth
    getBlueTeeth();
    if (MODE != 1)
      return;

    /*测试代码，之后删了
      if (startTime >= 60000)
      {
      MODE = 0;
      return;
      }
    */

  }
}

