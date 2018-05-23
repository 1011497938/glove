#include <Wire.h>

//x朝前，y向左

#define wGyro (10.0) //融合算法置信参数

boolean firstSample = true;

int lastTime = 0;
int interval = 0;

//重启传感器
void restart()
{
  initAcc();    //初始化加速度传感器
  initGyro();   //初始化角速度传感器
}


//得到传感器法向量，航向角
void getInclination() {
  int w = 0;
  float tmpf = 0.0;
  int currentTime, signRzGyro;

  float RwAcc[3];         // 用来计算倾角
  float Gyro[3];
  float RwGyro[3];        // 重新读取陀螺仪


  RwAcc[0] = acc[0];
  RwAcc[1] = acc[1];
  RwAcc[2] = acc[2];

  Gyro[0] = gyro[0];
  Gyro[1] = gyro[1];
  Gyro[2] = gyro[2];

  normalize3DVec(RwAcc);

  currentTime = millis();
  interval = currentTime - lastTime;
  lastTime = currentTime;

  if (firstSample) { // NaN用来等待检查从arduino过来的数据
    for (w = 0; w <= 2; w++) {
      RwEst[w] = RwAcc[w];    // 初始化加速度传感器读数
    }
  }
  else {
    // 对RwGyro进行评估
    if (abs(RwEst[2]) < 0.1) {
      // Rz值非常的小，它的作用是作为Axz与Ayz的计算参照值，防止放大的波动产生错误的结果。
      // 这种情况下就跳过当前的陀螺仪数据，使用以前的。
      for (w = 0; w <= 2; w++) {
        RwGyro[w] = RwEst[w];
      }
    }
    else {
      // ZX/ZY平面和Z轴R的投影之间的角度，基于最近一次的RwEst值
      for (w = 0; w <= 1; w++) {
        tmpf = Gyro[w];                        // 获取当前陀螺仪的deg/s
        tmpf *= interval / 1000.0f;                     // 得到角度变化值
        Awz[w] = atan2(RwEst[w], RwEst[2]) * 180 / PI;   // 得到角度并转换为度
        Awz[w] += tmpf;             // 根据陀螺仪的运动得到更新后的角度
      }

      // 判断RzGyro是多少，主要看Axz的弧度是多少
      // 当Axz在-90 ..90 => cos(Awz) >= 0这个范围内的时候RzGyro是准确的
      signRzGyro = ( cos(Awz[0] * PI / 180) >= 0 ) ? 1 : -1;

      // 从Awz的角度值反向计算RwGyro的公式
      for (w = 0; w <= 1; w++) {
        RwGyro[0] = sin(Awz[0] * PI / 180);

        RwGyro[0] /= sqrt( 1 + squared(cos(Awz[0] * PI / 180)) * squared(tan(Awz[1] * PI / 180)) );
        RwGyro[1] = sin(Awz[1] * PI / 180);
        RwGyro[1] /= sqrt( 1 + squared(cos(Awz[1] * PI / 180)) * squared(tan(Awz[0] * PI / 180)) );
      }
      RwGyro[2] = signRzGyro * sqrt(1 - squared(RwGyro[0]) - squared(RwGyro[1]));
    }

    // 把陀螺仪与加速度传感器的值进行结合
    for (w = 0; w <= 2; w++) RwEst[w] = (RwAcc[w] + wGyro * RwGyro[w]) / (1 + wGyro);

    normalize3DVec(RwEst);
  }

  firstSample = false;
}
