void getAccelValues() {
  sensors_event_t aevent;
  
  accel.getEvent(&aevent);
  
  ax = aevent.acceleration.x;
  ay = aevent.acceleration.y;
  az = -1 * aevent.acceleration.z;
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
