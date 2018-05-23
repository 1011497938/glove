//SZ-040蓝牙模块

//接受蓝牙信号，切换模式
void getBlueTeeth()
{
  if (Serial.available())
  {
    int i = Serial.read() - '0';
    if (i < MODE_NUMBER && i >= 0)
    {
      MODE = i;
      return;
    }
    else
      Serial.println("Can't find such mode");
  }
  return;   //没有查到模式返回
}

