// VARIABLES

const int samples = 300;
unsigned int samplesum;
double pitchRm[samples];  // Contains past values in order to filter out noise, with [0] being the most recent
double rollRm[samples];
double ratios[samples];

void setupRatiosVector(){
  samplesum = (samples + 1) * (samples / 2);
  for(int x=0; x<samples; x++) {
    ratios[x] = (double(samples - x) / double(samplesum));
  }
}

void reduceNoise(){
  for(int x=(samples - 1); x>0; x--) {
    pitchRm[x] = pitchRm[x-1];
    rollRm[x] = rollRm[x-1];
  }
  
  pitchRm[0] = pitch;
  rollRm[0] = roll;
  
  fpitch = 0.0;
  froll = 0.0;
  
  for(int x=0; x<samples; x++) {
    fpitch += pitchRm[x] * ratios[x];
    froll += rollRm[x] * ratios[x];
  }
  
  pitchRm[0] = fpitch;
  rollRm[0] = froll;
}
