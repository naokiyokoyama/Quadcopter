// (http://learn.parallax.com/KickStart/27911)
  

void getGyroValues() { // Calculates xdps/ydps/zdps and gx/gy/gz
  byte MSB, LSB;

  MSB = readI2C(0x29);
  LSB = readI2C(0x28);
  xRm = ((MSB << 8) | LSB); // Roll

  MSB = readI2C(0x2B);
  LSB = readI2C(0x2A);
  yRm = ((MSB << 8) | LSB); // Pitch

  MSB = readI2C(0x2D);
  LSB = readI2C(0x2C);
  zRm = ((MSB << 8) | LSB); // Yaw
}



void getGyroDPS() {
  if(abs(xRm - xRo) > xRth) {
    dR = xRm - xRo;
    xdps = SC * (float)dR;
  }
  else
  {xdps = 0;}
  
  if(abs(yRm - yRo) > yRth) {
    dR = yRm - yRo;
    ydps = -1 * SC * (float)dR;
  }
  else
  {ydps = 0;}
  
  if(abs(zRm - zRo) > zRth) {
    dR = zRm - zRo;
    zdps = SC * (float)dR;
  }
  else
  {zdps = 0;}
}

void getGyroAngles() {
  gx += xdps * (float)MicrosPassed / 1000000;
  gy += ydps * (float)MicrosPassed / 1000000;
  gz += zdps * (float)MicrosPassed / 1000000;
}

void ZeroRateLevelCalibration() {
  MillisTracker = millis();
  while(millis() < MillisTracker + MilliCalibrationTime) {
    getGyroValues();
    xRsum += xRm;
    yRsum += yRm;
    zRsum += zRm;
    C++;
  }
  xRo = xRsum / C;
  yRo = yRsum / C;
  zRo = zRsum / C;
}    
  
int readI2C (byte regAddr) {
    Wire.beginTransmission(Addr);
    Wire.write(regAddr);                // Register address to read
    Wire.endTransmission();             // Terminate request
    Wire.requestFrom(Addr, 1);          // Read a byte
    while(!Wire.available()) { };       // Wait for receipt
    return(Wire.read());                // Get result
}

void writeI2C (byte regAddr, byte val) {
    Wire.beginTransmission(Addr);
    Wire.write(regAddr);
    Wire.write(val);
    Wire.endTransmission();
}
