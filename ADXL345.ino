// 加速度传感器 ADXL345
#define ACC (0x53)    //定义ADXL345地址
#define A_TO_READ (6)        //读取每次占用的字节数 (每个坐标轴占两个字节)


// 加速度传感器误差修正的偏移量
#define A_OFFX  (-2)
#define A_OFFY  (-12)
#define A_OFFZ  (0)

//调用 ADXL345
void initAcc() {
  //调用 ADXL345
  writeTo(ACC, 0x2D, 0);
  writeTo(ACC, 0x2D, 16);
  writeTo(ACC, 0x2D, 8);
  //设定在 +-2g 时的默认读数
}


void getAccelerometerData()
{
  int regAddress = 0x32;    //加速度传感器ADXL345第一轴的数据的设定
  byte buff[A_TO_READ];
  float temp[3];
  readFrom(ACC, regAddress, A_TO_READ, buff); //读取加速度传感器ADXL345的数据

  //每个轴的读数有10位分辨率，即2个字节.
  //我们要转换两个bytes为一个int变量
  for ( int i = 0; i < 3; i++)
    temp[i] = acc[i];

  acc[0] = (((int)buff[1]) << 8) | buff[0] + A_OFFX;
  acc[1] = (((int)buff[3]) << 8) | buff[2] + A_OFFY;
  acc[2] = (((int)buff[5]) << 8) | buff[4] + A_OFFZ;

  if (acc[0] == 0 && acc[1] == 0 && acc[2] == 0)
  {
    for ( int i = 0; i < 3; i++)
      acc[i] = temp[i];
    return;
  }
  // 把原始数据转换为G
  acc[0] = float( acc[0]) / 256.0;
  acc[1] = float( acc[1]) / 256.0 / 1.04;
  acc[2] = float( acc[2]) / 256.0 * 1.15;

}





