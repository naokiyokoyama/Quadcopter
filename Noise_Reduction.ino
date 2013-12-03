// Multiply by 0.07 to get degrees
int Noise_Byte_Threshold = 45;             
int Noise_Byte_Difference_Threshold = 15; 
int xRm1, yRm1, zRm1, xRm2, yRm2, zRm2; 
unsigned int Reduced; // Amount of drastic noise removed
int xdps1, ydps1, zdps1;

void logGyroNoise() {
  xRm1 = xRm2;
  xRm2 = xRm;
  
  yRm1 = yRm2;
  yRm2 = yRm;
  
  zRm1 = zRm2;
  zRm2 = zRm;
}

void reduceGyroNoise() {
  if((
  (
  xRm2 < xRm - Noise_Byte_Threshold 
  && 
  xRm2 < xRm1 - Noise_Byte_Threshold 
  )
  ||
  (
  xRm2 > xRm + Noise_Byte_Threshold 
  && 
  xRm2 > xRm1 + Noise_Byte_Threshold 
  ))
  &&
  abs((xRm2 - xRm) - (xRm2 - xRm1)) < Noise_Byte_Difference_Threshold
  ) {
  xRm2 = (xRm + xRm1) / 2; Reduced++;}
  
  if((
  (
  yRm2 < yRm - Noise_Byte_Threshold 
  && 
  yRm2 < yRm1 - Noise_Byte_Threshold 
  )
  ||
  (
  yRm2 > yRm + Noise_Byte_Threshold 
  && 
  yRm2 > yRm1 + Noise_Byte_Threshold 
  ))
  &&
  abs((yRm2 - yRm) - (yRm2 - yRm1)) < Noise_Byte_Difference_Threshold
  )
  {yRm2 = (yRm + yRm1) / 2; Reduced++;}
  
  if((
  (
  zRm2 < zRm - Noise_Byte_Threshold 
  && 
  zRm2 < zRm1 - Noise_Byte_Threshold 
  )
  ||
  (
  zRm2 > zRm + Noise_Byte_Threshold 
  && 
  zRm2 > zRm1 + Noise_Byte_Threshold 
  ))
  &&
  abs((zRm2 - zRm) - (zRm2 - zRm1)) < Noise_Byte_Difference_Threshold
  )
  {zRm2 = (zRm + zRm1) / 2; Reduced++;}
}

void TransformGyroDPS() {
  int holder = xdps;
  ydps = xdps/sqrt(2) - ydps/sqrt(2);
  xdps = -1 * (holder/sqrt(2) + ydps/sqrt(2));  
}

void getGyroDPS2() {
  if(abs(xRm2 - xRo) > xRth) {
    dR = xRm2 - xRo;
    xdps = SC * (float)dR;
  }
  else
  {xdps = 0;}
  
  if(abs(yRm2 - yRo) > yRth) {
    dR = yRm2 - yRo;
    ydps = SC * (float)dR;
  }
  else
  {ydps = 0;}
  
  if(abs(zRm2 - zRo) > zRth) {
    dR = zRm2 - zRo;
    zdps = SC * (float)dR;
  }
  else
  {zdps = 0;}
  TransformGyroDPS();
}

void logMicrosPassed() {
  MicrosPassedPresent = MicrosPassed;
}

void logOldDPS() { 
  xdps1 = xdps;
  ydps1 = ydps;
  zdps1 = zdps;
}

void getGyroAngles2() { // Approximate integration using the Trapezoidal Rule
  gx += (((xdps + xdps1) * ((float)MicrosPassedPresent / 1000000)) / 2);
  gy += (((ydps + ydps1) * ((float)MicrosPassedPresent / 1000000)) / 2);
  gz += (((zdps + zdps1) * ((float)MicrosPassedPresent / 1000000)) / 2);
}
