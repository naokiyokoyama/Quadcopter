
void ZeroRateLevelCalibration() {
  MillisTracker = millis();
  while(millis() < MillisTracker + MilliCalibrationTime) {
    getGyroValues();
    getGyroDPS(xRm, yRm, zRm);
    xRmSum += xdps;
    yRmSum += ydps;
    zRmSum += zdps;
    ZRLCloops = ZRLCloops + 1.0;
  }
  xRo = xRmSum / ZRLCloops;
  yRo = yRmSum / ZRLCloops;
  zRo = zRmSum / ZRLCloops;
}   

void getGyroValues() {
  xRm = -readY();
  yRm = readX();
  zRm = -readZ();
}

void getGyroDPS(int XRM, int YRM, int ZRM) {
  xdps = SC * (double)XRM - xRo;
  ydps = SC * (double)YRM - yRo;
  zdps = SC * (double)ZRM - zRo;
}

void getGyroDPS1(int XRM, int YRM, int ZRM) {
  int dR;
  if(abs(XRM - xRo) > xRth) {
    dR = XRM - xRo;
    xdps = SC * (float)dR;
  }
  else
  {xdps = 0;}
  
  if(abs(YRM - yRo) > yRth) {
    dR = YRM - yRo;
    ydps = SC * (float)dR;
  }
  else
  {ydps = 0;}
  
  if(abs(ZRM - zRo) > zRth) {
    dR = ZRM - zRo;
    zdps = SC * (float)dR;
  }
  else
  {zdps = 0;}
}

void transformGyroDPS() {
  int oldydps = ydps;
  ydps = xdps/sqrt(2) - ydps/sqrt(2);
  xdps = -1 * (xdps/sqrt(2) + oldydps/sqrt(2));  
}

void logOldDPS() { 
  xdps1 = xdps;
  ydps1 = ydps;
  zdps1 = zdps;
}

void getGyroAngles() {
  gx = xdps * (float)MicrosPassed * 0.000001;
  gy = ydps * (float)MicrosPassed * 0.000001;
  gz = zdps * (float)MicrosPassed * 0.000001;
}

int readX(void)
{
  int data=0;
  data = itgRead(itgAddress, GYRO_XOUT_H)<<8;
  data |= itgRead(itgAddress, GYRO_XOUT_L);  
  
  return data;
}

int readY(void)
{
  int data=0;
  data = itgRead(itgAddress, GYRO_YOUT_H)<<8;
  data |= itgRead(itgAddress, GYRO_YOUT_L);  
  
  return data;
}

int readZ(void)
{
  int data=0;
  data = itgRead(itgAddress, GYRO_ZOUT_H)<<8;
  data |= itgRead(itgAddress, GYRO_ZOUT_L);  
  
  return data;
}



