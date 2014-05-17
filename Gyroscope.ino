
void ZeroRateLevelCalibration(boolean cond) {
  if(cond) {
    unsigned long ZRLCtracker = millis();
    long gyrosum[3];
    long ZRLCloops;
    while(millis() < ZRLCtracker + 2000) {
      int gyro[3];
      IMU.getGyroValues(gyro);
      for (int i=0;i<3;i++)
        gyrosum[i] += gyro[i];
      ZRLCloops++;
    }
    for (int i=0;i<3;i++)
      gyrozero[i] = (float)gyrosum[i] / (float)ZRLCloops;
  }
  else {
    gyrozero[0] = -0.2212387 - 0.0299484 - 0.5754207;
    gyrozero[1] = -0.0456476 - 0.0437478 - 0.4897250;
    gyrozero[2] = 0.3237676 + 0.7822324;
    thetazero[0] = 1.73 + 0.52 - 0.25123;
    thetazero[1] = 3.86 + 1.07 + 0.04;
  }
}   

//dpsX: -0.0299484	 dpsY: -0.0437478	
//dpsX: -0.5754207	 dpsY: -0.4897250	 dpsZ: 0.7822324

void getGyroDPS(float *array) {
  for (int i=0;i<3;i++)
    dps[i] = gyroSC * array[i];
  dps[0] *= -1;
}

void driftCompensation(float *array, float *zero) {
  for (int i=0;i<3;i++)
    array[i] -= zero[i];
}

void getGyroDPS2(float *current, float *old) {
  float timePassed = MicrosPassed * 0.000001;
  for (int i=0;i<3;i++)
    dps2[i] = (current[i]-old[i]) / timePassed;
}

void copyArray(float *dest, float *orig) { 
  for (int i=0;i<3;i++)
    dest[i] = orig[i];
}


