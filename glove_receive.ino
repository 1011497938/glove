/*接收到的各种传感器用于各模式的函数*/



//得到初始值
void  getOriginalVlaue()
{
  readSensors();
  for (int i; i < 3; i++)
  {
    old_acc[i] = acc[i];
    old_gyro[i] = gyro[i];
  }
  old_Heading = heading;
}

//基于方向角得到相对于初始角度的角度，负为逆时针，正为顺时针
int getRelativeAngle(int old_Heading, int heading)
{
  int deltaAngle = heading - old_Heading;

  if (deltaAngle <= 180 && deltaAngle >= -180) return deltaAngle;
  else if (deltaAngle < -180)  return deltaAngle + 360;
  else return deltaAngle - 360;
}

//得到按钮的按键状态
void get_touchButton_State()
{
  if (digitalRead(touchButtonPin[1]) == HIGH)
  {
    touchButton_State[1] = true;
    return;
  }
  else
    touchButton_State[1] = false;

  if (digitalRead(touchButtonPin[0]) == HIGH && touchButton_State[1] == false)
    touchButton_State[0] = true;
  else
    touchButton_State[0] = false;
}

void getFinger_State()
{
  int state;

  if (state = analogRead(fingerPin[2]) <= 590)
    Finger_State[2] = 1;
  else if (state <= 700 )
    Finger_State[2] = 2;
  else if (state <= 750 )
    Finger_State[2] = 3;
  else Finger_State[2] = 4;

  if (state = analogRead(fingerPin[1]) <= 590)
    Finger_State[1] = 1;
  else if (state <= 700 )
    Finger_State[1] = 2;
  else if (state <= 750 )
    Finger_State[1] = 3;
  else Finger_State[1] = 4;

  if (state = analogRead(fingerPin[0]) <= 620)
    Finger_State[0] = 1;
  else if (state <= 700 )
    Finger_State[0] = 2;
  else if (state <= 750 )
    Finger_State[0] = 3;
  else Finger_State[0] = 4;


}


void readSensors()
{
  getAccelerometerData();     //得到ADXL345数据
  getGyroscopeData();         //得到陀螺仪数据
  get_touchButton_State();        //得到按键状态
  getFinger_State();            //得到手指状态
  heading = getHeading();           //得到方向角
}

//这一功能在阴暗条件下效果并不好，但是基本没有问题
bool getLigterSensor(int SensorPin )
{
  if ( analogRead( SensorPin ) >= 990 )
    return false;   //表示手掌是贴着桌面的
  else return true;  //表示手掌在空中
}
//判断是否有横向滚动的
unsigned long RollFirstTime;
bool RolldelayTime;
bool isRoll()
{
  if( millis() - RolldelayTime <= 500 )
    return false;
  else
    RolldelayTime = 0;
    
  if( fabs(gyro[0]) >= 30 )
  {
    if( RollFirstTime == 0 )
      RollFirstTime = millis();
    else if( millis() - RollFirstTime >= 300 )
    {
      RolldelayTime = millis();
      RollFirstTime = 0;
      return true;
    }
    else  return false;
  }
  else
  {
    RollFirstTime = 0;
    return false;
  }
  
}

//判断是否有极速上抬
unsigned long JumpFirstTime;
bool JumpdelayTime;
bool isJump()
{
  if( millis() - JumpdelayTime <= 300 )
    return false;
  else
    JumpdelayTime = 0;
    
  if( fabs(gyro[1]) >= 30 )
  {
    if( JumpFirstTime == 0 )
      JumpFirstTime = millis();
    else if( millis() - JumpFirstTime >= 300 )
    {
      JumpdelayTime = millis();
      JumpFirstTime = 0;
      return true;
    }
    else  return false;
  }
  else
  {
    JumpFirstTime = 0;
    return false;
  }
  
}

//判断是否有前推
unsigned long PushFirstTime;
bool PushdelayTime;
bool isPush()
{
  if( millis() - PushdelayTime <= 300 )
    return false;
  else
    PushdelayTime = 0;
    
  if( fabs(acc[0]) >= 0.5 )
  {
    if( PushFirstTime == 0 )
      PushFirstTime = millis();
    else if( millis() - PushFirstTime >= 300 )
    {
      PushdelayTime = millis();
      PushFirstTime = 0;
      return true;
    }
    else  return false;
  }
  else
  {
    PushFirstTime = 0;
    return false;
  }
  
}

//马达震动
void  vibrate()
{
  digitalWrite(  vibratePin, 0);
  delay(100);
  digitalWrite(  vibratePin, 255);
}

