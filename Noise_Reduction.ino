// Multiply by SC (1/14.375) to get degrees
int Noise_Byte_Threshold = 45;             
int Noise_Byte_Difference_Threshold = 15; 
unsigned int Reduced; // Amount of drastic noise removed


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

void getGyroDPS2() {
  int dR;
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
}

void logMicrosPassed() {
  MicrosPassedPresent = MicrosPassed;
}



