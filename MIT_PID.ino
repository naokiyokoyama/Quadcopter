
int speeds[4];

int MaxWave2 = 150;        // Upper speed limit
int MinWave2 = 40;         // Lower speed limit
int HoverThrottle2 = 100;  // Speed of motor when level

double rollin = 0.0;
double pitchin = 0.0;

double p = 0.256;
double i = 0.00;
double d = 0.00;
double period = 0.0;

double droll, iroll, dpitch, ipitch;

void MITPID() {
  deltaPID();
  
  for(int x=0; x<4; x++) {
    if(speeds[x]>MaxWave2) speeds[x]=MaxWave2;
    if(speeds[x]<MinWave2) speeds[x]=MinWave2;
  }
  
  West.write(speeds[1]);
  East.write(speeds[0]);
}

void simplePD() {
  speeds[0] = HoverThrottle2 - p*(roll - rollin) - d*xdps;
  speeds[1] = HoverThrottle2 - p*(rollin - roll) + d*xdps;
  speeds[2] = HoverThrottle2 - p*(pitch - pitchin) - d*ydps;
  speeds[3] = HoverThrottle2 - p*(pitchin - pitch) + d*ydps;
}

void deltaPID() {
  droll = (roll - oldroll) / ( (double) MicrosPassed / 1000000.0);
  dpitch = (pitch - oldpitch)/ ( (double) MicrosPassed / 1000000.0);
  iroll += DeltaTrapezoidalRule( oldroll - rollin, roll - rollin);
  ipitch += DeltaTrapezoidalRule( oldpitch - pitchin, pitch - pitchin);
  speeds[0] = HoverThrottle2 - p*(roll - rollin) - i*iroll - d*droll;
  speeds[1] = HoverThrottle2 + p*(roll - rollin) + i*iroll + d*droll;
  speeds[2] = HoverThrottle2 - p*(pitch - pitchin) - i*ipitch - d*dpitch;
  speeds[3] = HoverThrottle2 + p*(pitch - pitchin) + i*ipitch + d*dpitch;
}

void SetTuningsClassicPID() {
 p = 0.6 * p;
 i = 2.0 * p / period;
 d = p * period / 8.0;
}

void SetTuningsPessenIntegralRule() {
 p = 0.7 * p;
 i = 2.5 * p / period;
 d = p * period * 0.15;
}

void SetTuningsSomeOvershoot() {
 p = 0.33 * p;
 i = 2.0 * p / period;
 d = p * period / 3.0;
}

void SetTuningsClassicPID() {
 p = 0.2 * p;
 i = 2.0 * p / period;
 d = p * period / 8.0;
}
 
 
