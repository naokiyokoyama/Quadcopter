void getAccelValues() {
  sensors_event_t aevent;
  accel.getEvent(&aevent);
  
  ax = aevent.acceleration.x;
  ay = aevent.acceleration.y;
  az = -1 * aevent.acceleration.z;

  ar = sqrt(ax*ax + ay*ay + az*az);
  
  thetaX = 90.0 - (acos(ax/ar)*(180.0/PI));
  thetaY = 90.0 - (acos(ay/ar)*(180.0/PI));
  thetaZ = acos(az/ar)*(180.0/PI);
  
  ax2 = ax/sqrt(2) - ay/sqrt(2);
  ay2 = ax/sqrt(2) + ay/sqrt(2);
  ar2 = sqrt(ax2*ax2 + ay2*ay2 + az*az);
  
  thetaX2 = -1 * (90.0 - (acos(ax2/ar2)*(180.0/PI)));
  thetaY2 = 90.0 - (acos(ay2/ar2)*(180.0/PI));
}

long pitchsum, rollsum;
float LevelRoll, LevelPitch;

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

int readI2C (int Address, byte regAddr) {
    Wire.beginTransmission(Address);
    Wire.write(regAddr);                // Register address to read
    Wire.endTransmission();             // Terminate request
    Wire.requestFrom(Address, 1);          // Read a byte
    while(!Wire.available()) { };       // Wait for receipt
    return(Wire.read());                // Get result
}

void writeI2C (int Address, byte regAddr, byte val) {
    Wire.beginTransmission(Address);
    Wire.write(regAddr);
    Wire.write(val);
    Wire.endTransmission();
}
