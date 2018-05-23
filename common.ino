//求平方和用来判断加速度是否仅为重力加速度
float quadratic_sum( float x[])
{
  return x[0] * x[0] + x[1] * x[2] + x[3] * x[3];
}

//求平方
float squared(float x) {
  return x * x;
}

//单位化向量
void normalize3DVec(float vector[]) {
  float R;
  R = sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
  vector[0] /= R;
  vector[1] /= R;
  vector[2] /= R;
}

// 将值写入累加器中的地址寄存器(ADXL345和ITG3205都要用到）
void writeTo(int DEVICE, byte address, byte val) {
  Wire.beginTransmission(DEVICE); // 开始传输数据到累加器
  Wire.write(address);        //  发送寄存器地址
  Wire.write(val);        //  发送要写入的值
  Wire.endTransmission(); // 结束传输
}



// 从累加器的地址寄存器中读取buff数组数据(ADXL345和ITG3205都要用到）
void readFrom(int DEVICE, byte address, int num, byte buff[]) {
  Wire.beginTransmission(DEVICE); // 开始传输数据到累加器
  Wire.write(address);        // 发送读取的地址
  Wire.endTransmission(); // 结束传输

  Wire.beginTransmission(DEVICE); // 开始传输数据到累加器
  Wire.requestFrom(DEVICE, num);    //  从累加器请求6个字节数据

  int i = 0;
  while (Wire.available())
  {
    buff[i] = Wire.read(); // 接收一个字节
    i++;
  }
  Wire.endTransmission(); // 结束传输
}

//限幅滤波
bool filter(float n, float MAX, float MIN)  //判断数n
{
  if (n <= MAX && n >= MIN)
    return true;
  else
    return false;
}



