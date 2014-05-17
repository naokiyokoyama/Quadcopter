
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
