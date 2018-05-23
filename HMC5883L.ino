#define HMC5883L 0x1E //001 1110b(0x3C>>1), I2C 7bit HMC5883L of HMC5883
#define MagnetcDeclination 4.43 //Hangzhou
#define CalThreshold 0

int offsetX;
int offsetY;
int offsetZ;

//初始化HMC5883L
void initHea()
{
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(HMC5883L); //open communication with HMC5883r
  Wire.write(0x00); //select configuration register A
  Wire.write(0x70); //0111 0000b configuration
  Wire.endTransmission();

  Wire.beginTransmission(HMC5883L);
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //set continuous measurement mode:0x00,single-measurement mode:0x01
  Wire.endTransmission();

}

int getHeading()
{
  int x, y, z;
  //Tell the HMC5883L where to begin reading data
  Wire.beginTransmission(HMC5883L);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();
  //Read data from each axis, 2 registers per axis
  Wire.requestFrom(HMC5883L, 6);
  if (6 <= Wire.available()) {
    x = Wire.read() << 8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read() << 8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read() << 8; //Y msb
    y |= Wire.read(); //Y lsb
  }

  return calculateHeading(x, y, z);
}


//得到结果为y轴负方向与南的顺时针角，只能得到水平时的值非常不准，三维算法连续出错已经放弃，现在只用ITG3205测z轴角度的
int calculateHeading(int x , int y, int z)
{
  float headingRadians = atan2((double)((y) - offsetY), (double)((x) - offsetX));
  // Correct for when signs are reversed.
  if (headingRadians < 0)
    headingRadians += 2 * PI;

  int headingDegrees = headingRadians * 180 / M_PI;
  headingDegrees += MagnetcDeclination; //the magnetc-declination angle

  // Check for wrap due to addition of declination.
  if (headingDegrees > 360)
    headingDegrees -= 360;

  return headingDegrees;
}

//校准磁力(二十秒将传感器旋转一周）
void calibrateMag()
{
  int x, y, z; //triple axis data
  int xMax, xMin, yMax, yMin, zMax, zMin;
  //initialize the variables
  //Tell the HMC5883L where to begin reading data
  Wire.beginTransmission(HMC5883L);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();
  //Read data from each axis, 2 registers per axis
  Wire.requestFrom(HMC5883L, 6);
  if (6 <= Wire.available()) {
    x = Wire.read() << 8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read() << 8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read() << 8; //Y msb
    y |= Wire.read(); //Y lsb
  }

  xMax = xMin = x;
  yMax = yMin = y;
  zMax = zMin = z;
  offsetX = offsetY = offsetZ = 0;

  Serial.println("Please turn around in 20 seconds");

  for (int i = 0; i < 200; i++)
  {
    if (i % 10 == 0)
    {
      Serial.println(i / 10);
    }
    //Tell the HMC5883L where to begin reading data
    Wire.beginTransmission(HMC5883L);
    Wire.write(0x03); //select register 3, X MSB register
    Wire.endTransmission();
    //Read data from each axis, 2 registers per axis
    Wire.requestFrom(HMC5883L, 6);
    if (6 <= Wire.available()) {
      x = Wire.read() << 8; //X msb
      x |= Wire.read(); //X lsb
      z = Wire.read() << 8; //Z msb
      z |= Wire.read(); //Z lsb
      y = Wire.read() << 8; //Y msb
      y |= Wire.read(); //Y lsb
    }
    //get Max and Min
    // this routine will capture the max and min values of the mag X, Y, and Z data while the
    // compass is being rotated 360 degrees through the level plane and the upright plane.
    // i.e. horizontal and vertical circles.
    // This function should be invoked while making continuous measurements on the magnetometers
    if (x > xMax)
      xMax = x;
    if (x < xMin )
      xMin = x;
    if (y > yMax )
      yMax = y;
    if (y < yMin )
      yMin = y;
    if (z > zMax )
      zMax = z;
    if (z < zMin )
      zMin = z;

    delay(100);

  }
  //compute offsets
  if (abs(xMax - xMin) > CalThreshold )
    offsetX = (xMax + xMin) / 2;
  if (abs(yMax - yMin) > CalThreshold )
    offsetY = (yMax + yMin) / 2;
  if (abs(zMax - zMin) > CalThreshold )
    offsetZ = (zMax + zMin) / 2;

}

