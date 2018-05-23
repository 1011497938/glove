/*预设模式
   Mode0 暂停模式
   Mode1 常用鼠标模式（默认模式）
   Mode2 第一人称类模式（姿态控制上下，滚轮控制左右，点击射击瞄准）
   Mode3 空中遥感模式，带四个键
*/


unsigned long  StartMinute = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("GLOVE START");

  Wire.begin();
  Mouse.begin();
  Keyboard.begin();
  initAcc();    //初始化加速度传感器
  initGyro();   //初始化角速度传感器

  pinMode(switchPin, INPUT);
  pinMode(touchButtonPin[0], INPUT);
  pinMode(touchButtonPin[1], INPUT);
  pinMode(fingerPin[0], INPUT);
  pinMode(fingerPin[1], INPUT);
  pinMode(fingerPin[2], INPUT);

  Serial.println("READY");
}


void loop()
{
  //计时
  if ( micros() > 4294967295 - 10000 )
    StartMinute += 71583;

  switch(MODE)
  {
    case 0: Mode0();break;
    case 1: Mode1();break;
    case 2: Mode2();break;
    case 3: Mode3();break;
    default: Mode0();break;
  }

  getBlueTeeth();
}






