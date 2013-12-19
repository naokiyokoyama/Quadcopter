// VARIABLES

const int samples = 100;
unsigned int samplesum;
double thetaXRm[samples];  // Contains past values in order to filter out noise, with [0] being the most recent
double thetaYRm[samples];
double ratios[samples];

void setupRatiosVector(){
  samplesum = (samples + 1) * (samples / 2);
  for(int x=0; x<samples; x++) {
    ratios[x] = (double(samples - x) / double(samplesum));
  }
}

void reduceNoise(){
  for(int x=(samples - 1); x>0; x--) {
    thetaXRm[x] = thetaXRm[x-1];
    thetaYRm[x] = thetaYRm[x-1];
  }
  
  thetaXRm[0] = thetaX;
  thetaYRm[0] = thetaY;
  
  fthetaX = 0.0;
  fthetaY = 0.0;
  
  for(int x=0; x<samples; x++) {
    fthetaX += thetaXRm[x] * ratios[x];
    fthetaY += thetaYRm[x] * ratios[x];
  }
  
//  thetaXRm[0] = fthetaX;
//  thetaYRm[0] = fthetaY;
}
