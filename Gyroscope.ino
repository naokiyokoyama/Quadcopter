


void getGyroAngles() {
  gx = xdps * (float)MicrosPassed / 1000000.0;
  gy = ydps * (float)MicrosPassed / 1000000.0;
  gz = zdps * (float)MicrosPassed / 1000000.0;
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

