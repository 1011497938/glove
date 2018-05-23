//暂停模式
void Mode0()
{
  Serial.println("MODE0 START");
  while(1)
  {
    getBlueTeeth();
    if (MODE != 0)
      return;    
  }
}

