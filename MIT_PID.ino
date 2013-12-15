
int speeds[4];

int MaxWave2 = 150;        // Upper speed limit
int MinWave2 = 40;         // Lower speed limit
int HoverThrottle2 = 100;  // Speed of motor when level

double rollin = 0.0;
double pitchin = 0.0;

double p = 0.256;
double i = 0.03;
double d = 0.079;


void MITPID() {
  deltaPID();
  
  for(int x=0; x<4; x++) {
    if(speeds[x]>MaxWave2) speeds[x]=MaxWave2;
    if(speeds[x]<MinWave2) speeds[x]=MinWave2;
  }
  
  West.write(speeds[1]);
  East.write(speeds[0] + 1);
}

void simplePD() {
  speeds[0] = HoverThrottle2 - p*(roll - rollin) - d*xdps;
  speeds[1] = HoverThrottle2 - p*(rollin - roll) + d*xdps;
  speeds[2] = HoverThrottle2 - p*(pitch - pitchin) - d*ydps;
  speeds[3] = HoverThrottle2 - p*(pitchin - pitch) + d*ydps;
}

void deltaPID() {
  speeds[0] = HoverThrottle2 - (int) ( p*(roll - rollin) - i*(roll - oldroll) * ( (double) MicrosPassed / 1000000) ) - d*(roll - oldroll)/ ( (double) MicrosPassed / 1000000) ;
  speeds[1] = HoverThrottle2 - (int) ( p*(rollin - roll) + i*(roll - oldroll) * ( (double) MicrosPassed / 1000000) ) + d*(roll - oldroll)/ ( (double) MicrosPassed / 1000000) ;
  speeds[2] = HoverThrottle2 - (int) ( p*(pitch - pitchin) - i*(pitch - oldpitch) * ( (double) MicrosPassed / 1000000) ) - d*(pitch - oldpitch)/ ( (double) MicrosPassed / 1000000) ;
  speeds[3] = HoverThrottle2 - (int) ( p*(pitchin - pitch) + i*(pitch - oldpitch) * ( (double) MicrosPassed / 1000000) ) + d*(pitch - oldpitch)/ ( (double) MicrosPassed / 1000000) ;
}
