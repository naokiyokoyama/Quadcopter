
// GENERAL

const float CFRatio = 0.98;

void ComplementaryFilter() {
  oldpitch = pitch;
  oldroll  = roll;
  pitch = (CFRatio)*(pitch + gtrap[1])+(1.0 - CFRatio)*(theta[1]);
  roll  = (CFRatio)*(roll  + gtrap[0])+(1.0 - CFRatio)*(theta[0]);
}

float DeltaTrapezoidalRule(float old, float current) { // Approximate integration using the Trapezoidal Rule
  float trapezoid = (old + current) * (float)MicrosPassed * 0.0000005;
  return trapezoid; 
}

void copyArray(float *dest, float *orig) { 
  for (int i=0;i<3;i++)
    dest[i] = orig[i];
}

void transform(float *array, int startelement) {
  float hold = array[startelement+1];
  array[startelement+1] = array[startelement]/sqrt2 - array[startelement+1]/sqrt2;
  array[startelement] = -1 * (array[startelement]/sqrt2 + hold/sqrt2); 
}

void transformint(int *array, int startelement) {
  float hold = float(array[startelement]);
  array[startelement]   = int(float(array[startelement])/sqrt2 - float(array[startelement+1])/sqrt2);
  array[startelement+1] = int(float(hold)/sqrt2 + array[startelement+1]/sqrt2); 
}

// ACCELEROMETER

void getAccelAngles(float *array) {
  float ar = sqrt(array[0]*array[0] + array[1]*array[1] + array[2]*array[2]);
  
  theta[0] = 90.0 - (acos(array[0]/ar)*(180.0/PI));
  theta[1] = 90.0 - (acos(array[1]/ar)*(180.0/PI));
  theta[2] = acos(array[2]/ar)*(180.0/PI);
  theta[0] -= thetazero[0];
  theta[1] -= thetazero[1];
}

// GYROSCOPE

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
    thetazero[0] = 1.9814934 + 1.8939931;
    thetazero[1] = 3.86 + 1.07 + 0.04;
  }
}   

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

// MAGNETOMETER

void findOffsets(int *array) {
  Serial.begin(9600);
  int maximum[3], minimum[3], mag[3];
  IMU.getMagValues(mag);
  Serial.println("Ready for action!! Let's calibrate the magnetometer.");
  while(!Serial.available()) {
    
    IMU.getMagValues(mag);
    
    for (int i=0;i<3;i++) {
      if (mag[i] > maximum[i])
        maximum[i] = mag[i];
        
      if (mag[i] < minimum[i])
        minimum[i] = mag[i];
    }
    
    static unsigned long PrintTracker;
    const unsigned int MilliPrintTime = 100;  
  
    if(millis() >= PrintTracker) {
       PrintTracker = (millis() + MilliPrintTime);
       Serial.print(" mx: "); Serial.print(mag[0]); Serial.print("\t");
       Serial.print(" my: "); Serial.print(mag[1]); Serial.print("\t");
       Serial.print(" mz: "); Serial.print(mag[2]); Serial.print("\t");
       Serial.println();
    }
  }
  for (int i=0;i<3;i++) {
    array[i] = minimum[i] + abs(maximum[i] - minimum[i]) / 2;
    Serial.println(array[i]);
  }
  Serial.end();  
}

void centerMagValues(int *array) {
  for (int i=0;i<3;i++)
    array[i] -= magoffsets[i]; 
}

float getMagHeading(int *array) {
  float newpitch = roll * 0.017453;
  float newroll = pitch * 0.017453;
  
  float truex = (float)array[0] * cos(newpitch) + (float)array[2] * sin(newpitch);
  float truey = (float)array[0] * sin(newroll) * sin(newpitch) + (float)array[1] * cos(newroll) - (float)array[2] * sin(newroll) * cos(newpitch);
  
  float heading = atan2(truey, truex) * 57.29577;
  if(heading < 0) heading += 360.0;
  
  return heading;
}

//void getYaw(){
//  double gyroyaw = yaw + gz;
//  
//  if(gyroyaw < 0.0) gyroyaw += 360.0;
//  if(gyroyaw > 360.0) gyroyaw -= 360; 
//  
//  yaw = (YawRatio) * (gyroyaw) + (1.0 - YawRatio) * (heading); 
//  
//  if(abs(gyroyaw - heading) > 180) 
//  yaw = gyroyaw;
//}
