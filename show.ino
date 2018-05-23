//用于调试的函数

void showGy85Delta()
{
  Serial.print("  Xacc:");
  Serial.print(acc[0]);
  Serial.print("  Yacc:");
  Serial.print(acc[1]);
  Serial.print("  Zacc:");
  Serial.print(acc[2]);
  Serial.println();
  Serial.print("  Xgyro:");
  Serial.print(gyro[0]);
  Serial.print("  Ygyro:");
  Serial.print(gyro[1]);
  Serial.print("  Zgyro:");
  Serial.print(gyro[2]);
  Serial.println();
}

void showInclination()
{
  Serial.print("  RwEstX:");
  Serial.print(RwEst[0]);
  Serial.print("  RwEstY:");
  Serial.print(RwEst[1]);
  Serial.print("  RwEstZ:");
  Serial.print(RwEst[2]);
  Serial.print("  Heading:");
  Serial.print(getRelativeAngle(old_Heading , heading));
  Serial.println();
}

void showFingerAndButton()
{
  Serial.print(touchButton_State[1]);
  Serial.print("  ");
  Serial.print(touchButton_State[0]);
  Serial.print("  ");
  Serial.print(Finger_State[1]);
  Serial.print("  ");
  Serial.print(Finger_State[0]);
  Serial.println();
}

