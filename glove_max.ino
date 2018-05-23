#include <Keyboard.h>
#include <Wire.h>
#include <Mouse.h>

/*
   备忘录：
   初始化时按钮和手指的pinMode要设置成input
   需要研究pin值大.小
*/

#define MODE_NUMBER 4

float old_acc[3];       //初始加速度值，x对0，y对1，z对2
float  old_gyro[4];      //初始角速度值
int old_Heading;   //初始方向角

//直接观测结果
float acc[3];       //加速度值
float  gyro[4];      //角速度值
int heading;       //方向角，传感器的水平情况，有有上下时不稳定，需改进

bool touchButton_State[2] = { false , false }; //靠指尖处触摸传感器状态为0，非靠指尖处触摸传感器状态为1
int Finger_State[3] = { 0 , 0 , 0};  //中指状态为 Finger_State[0],食指位 Finger_State[1],无名指为 Finger_State[2],数值越高握紧程度越大
bool switch_State = true; //食指处开关状态，true为开

//使用void getInclination(float *RwEst)函数得出
float  RwEst[3];          // 传感器法向量,记录姿态
float Awz[3];           // XZ/ YZ平面和Z轴（度）R的投影之间的角度，Awz[0]为俯仰角Pitch,及鼠标的高度角,Awz[1]为y与xy的夹角（横滚角）Roll，角度制

int MODE = 1;

//Pin
const int touchButtonPin[2] = { A2, A3};  //紫线为A3，黄线为A2
const int switchPin = 5;  //开关
const int fingerPin[3] = {A0, A1, A4}; //中指A0，食指A1,无名指A4
const int lightSensorPin = A5;  //手掌底部传感器，用来检测手掌是不是贴地
const int  vibratePin = 4;

//Mode里的全局变量
unsigned long MouseTime = 0,  //上一次操作时间
              startTime = 0,
              clickTime = 0;
//控制时间中断，间隔太短传到电脑会使电脑卡死



