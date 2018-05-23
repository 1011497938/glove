// 定义ITG3200陀螺仪常量
#define GYRO 0x68 // 设置IIC地址,AD0与GND相接,二进制数值为11101000.
#define G_SMPLRT_DIV 0x15
#define G_DLPF_FS 0x16
#define G_INT_CFG 0x17
#define G_PWR_MGM 0x3E

#define G_TO_READ 8 // x, y, z 每个轴输出为2个字节

//  XYZ三轴偏移量修正
#define g_offx  (-70) // 修正X轴误差
#define g_offy  (68)   // 修正Y轴误差
#define g_offz  (1)   // 修正Z轴误差

int hx, hy, hz, turetemp;

// 初始化陀螺仪
void initGyro()
{
  writeTo(GYRO, G_PWR_MGM, 0x00);
  writeTo(GYRO, G_SMPLRT_DIV, 0x07); // EB, 50, 80, 7F, DE, 23, 20, FF
  writeTo(GYRO, G_DLPF_FS, 0x1E); // +/- 2000 dgrs/sec, 1KHz, 1E, 19
  writeTo(GYRO, G_INT_CFG, 0x00);
}


void getGyroscopeData()
{

  int regAddress = 0x1B;
  int temp, x, y, z;
  byte buff[G_TO_READ];

  readFrom(GYRO, regAddress, G_TO_READ, buff); // 从ITG3200读取数据

  gyro[0] = ((buff[2] << 8) | buff[3]) + g_offx;
  gyro[1] = ((buff[4] << 8) | buff[5]) + g_offy;
  gyro[2] = ((buff[6] << 8) | buff[7]) + g_offz;
  gyro[3] = (buff[0] << 8) | buff[1]; // 温度

  gyro[0] = gyro[0] / 14.375 ; // 输出值转换为角速度,单位是度/秒
  gyro[1] =  gyro[1] / 14.375 ;
  gyro[2] =  gyro[2] / 14.375 ;
  gyro[3] = 35 + ((double) (gyro[3] + 13200)) / 280; // 输出值转换为摄氏度
}






