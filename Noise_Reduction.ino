// VARIABLES

const int samples = 10;
unsigned int samplesum;
double axRm[samples];  // Contains past values in order to filter out noise, with [0] being the most recent
double ayRm[samples];
double azRm[samples];
double thetaXRm[samples];
double thetaYRm[samples];
double ratios[samples];
double axsum, aysum, azsum, thetaXsum, thetaYsum;

boolean SetupRatiosVector = false;

void setupRatiosVector(){
  samplesum = (samples + 1) * (samples / 2);
  for(int x=0; x<samples; x++) {
    ratios[x] = (double(samples - x) / double(samplesum));
  }
}

void reduceNoise(){
  if(!SetupRatiosVector){
    setupRatiosVector();
    SetupRatiosVector = true;
  }
  
  for(int x=(samples - 1); x>0; x--) {
    axRm[x] = axRm[x-1];
    ayRm[x] = ayRm[x-1];
    azRm[x] = azRm[x-1];
  }
  
  axRm[0] = ax;
  ayRm[0] = ay;
  azRm[0] = az;
  
  fax = 0.0;
  fay = 0.0;
  faz = 0.0;
  
  for(int x=0; x<samples; x++) {
    fax += axRm[x] * ratios[x];
    fay += ayRm[x] * ratios[x];
    faz += azRm[x] * ratios[x];
  }
  
  axRm[0] = fax;
  ayRm[0] = fay;
  azRm[0] = faz;
}

void reduceNoiseTheta(){
  if(!SetupRatiosVector){
    setupRatiosVector();
    SetupRatiosVector = true;
  }
  
  for(int x=(samples - 1); x>0; x--) {
    thetaXRm[x] = thetaXRm[x-1];
  }
  
  thetaXRm[0] = thetaX;
  
  thetaX = 0.0;
  
  for(int x=0; x<samples; x++) {
    thetaX += thetaXRm[x] * ratios[x];
  }
  
  thetaXRm[0] = thetaX;
}

void simpleMovingAverageAccel() {
  for(int x=(samples - 1); x>0; x--) {
    axRm[x] = axRm[x-1];
    ayRm[x] = ayRm[x-1];
    azRm[x] = azRm[x-1];
  }
  
  axRm[0] = ax;
  ayRm[0] = ay;
  azRm[0] = az;
  
  axsum = 0.0;
  aysum = 0.0;
  azsum = 0.0;
  
  for(int x=0; x<samples; x++) {
    axsum += axRm[x];
    aysum += ayRm[x];
    azsum += azRm[x];
  }
  
  fax = axsum / samples;
  fay = aysum / samples;
  faz = azsum / samples;
}

void simpleMovingAverageTheta() {
  for(int x=(samples - 1); x>0; x--) {
    thetaXRm[x] = thetaXRm[x-1];
    thetaYRm[x] = thetaYRm[x-1];
  }
  
  thetaXRm[0] = thetaX;
  thetaYRm[0] = thetaY;
  
  thetaXsum = 0.0;
  thetaYsum = 0.0;  
  
  for(int x=0; x<samples; x++) {
    thetaXsum += thetaXRm[x];
    thetaYsum += thetaYRm[x];
  }
  
  thetaX = thetaXsum / samples;
  thetaY = thetaYsum / samples;
  
  thetaXRm[0] = thetaX;
  thetaYRm[0] = thetaY;
}
