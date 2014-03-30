const double conversionFactor = double(SCALE / pow(2, 15));

void initLSM303(int fs)
{
  itgWrite(LSM303_ACC, CTRL_REG1_A, 0b10010111);  // low power mode, 5.376 kHz, all accel axes on
  switch (fs) {
    case 2:
    itgWrite(LSM303_ACC, CTRL_REG4_A, 0b00000000);
    break;
    case 4:
    itgWrite(LSM303_ACC, CTRL_REG4_A, 0b00010000);
    break;
    case 8:
    itgWrite(LSM303_ACC, CTRL_REG4_A, 0b00100000);
    break;
  }
  itgWrite(LSM303_MAG, CRA_REG_M, 0b00011100); 
  itgWrite(LSM303_MAG, MR_REG_M, 0b00000000);  
}

void getAccelValues() {
  int z = -((int)LSM303_read(OUT_X_L_A) << 8) | (LSM303_read(OUT_X_H_A));
  int x = ((int)LSM303_read(OUT_Y_L_A) << 8) | (LSM303_read(OUT_Y_H_A));
  int y = ((int)LSM303_read(OUT_Z_L_A) << 8) | (LSM303_read(OUT_Z_H_A));  
  
  az = z * conversionFactor;
  ax = x * conversionFactor;
  ay = y * conversionFactor;
  // had to swap those to right the data with the proper axis
}

void getAccelAngles(double accelX, double accelY, double accelZ) {
  ar = sqrt(accelX*accelX + accelY*accelY + accelZ*accelZ);
  
  thetaX = 90.0 - (acos(accelX/ar)*(180.0/PI));
  thetaY = 90.0 - (acos(accelY/ar)*(180.0/PI));
  thetaZ = acos(accelZ/ar)*(180.0/PI);
}

long pitchsum, rollsum;

void AccelLevelCalibration() {
  unsigned long loops;
  pitchsum = 0;
  rollsum = 0;
  MillisTracker = millis();
  while(millis() < MillisTracker + MilliCalibrationTime) {
    pitchsum += pitch;
    rollsum += roll;
    loops++;
  }
  LevelPitch = pitchsum / loops;
  LevelRoll = rollsum / loops;
}   



