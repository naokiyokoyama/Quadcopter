
int speeds[4];

int MaxWave2 = 179;        // Upper speed limit
int MinWave2 = 40;         // Lower speed limit
int HoverThrottle2 = 100;  // Speed of motor when level

double rollin = 0.0;
double pitchin = 0.0;//-48.4;

double p = 0.5538;//0.6; //.36
double i = 0.852;//0.099;//0.9527812;
double d = 0.09;//0.11;//1444 900 2020 0 1000

double yp = 0.197; //.36
double yd = 0.03;

double period = 1.379;

double droll, iroll, dpitch, ipitch;

void MITPID() {
  pitchin = double(map(LeftVertical(), -500, 500, -15, 15) * 2);
  rollin = double(map(LeftHorizontal(), -500, 500, -15, 15) * 2);
  
  HoverThrottle2 = map(RightVertical(), 0, 1010, 30, 150);

//  HoverThrottle2 = 120;
  
//  p = double(map(RightVerticalVolatile, 900, 2020, 2000, 3500)) / 1000.0;
  
//  if(p<0) p = 0.0;

  if(HoverThrottle2<30) HoverThrottle2=30;
  
  deltaPID();
  
  for(int x=0; x<4; x++) {
    if(speeds[x]>MaxWave2) speeds[x]=MaxWave2;
    if(speeds[x]<MinWave2) speeds[x]=MinWave2;
  }
  
  East.write(speeds[0]);
  West.write(speeds[1]);
  North.write(speeds[2]);
  South.write(speeds[3]);
}

void deltaPID() {
//  droll = (roll - oldroll)/ ( (double) MicrosPassed / 1000000.0);
//  dpitch = (pitch - oldpitch)/ ( (double) MicrosPassed / 1000000.0);
  
  droll = xdps;
  dpitch = ydps;
  
  double dyaw = (zdps - zdps1)/( (double) MicrosPassed / 1000000.0);
  
  if(pitchin != pitch){
    iroll += DeltaTrapezoidalRule( oldroll - rollin, roll - rollin);
  }
  if(rollin !=roll){
    ipitch += DeltaTrapezoidalRule( oldpitch - pitchin, pitch - pitchin);
  }
  
  speeds[0] = int(double(HoverThrottle2) - p*(roll - rollin) - i*iroll - d*droll + yp*zdps + yd*dyaw);
  speeds[1] = int(double(HoverThrottle2) + p*(roll - rollin) + i*iroll + d*droll + yp*zdps + yd*dyaw);
  speeds[2] = int(double(HoverThrottle2) - p*(pitch - pitchin) - i*ipitch - d*dpitch - yp*zdps - yd*dyaw);
  speeds[3] = int(double(HoverThrottle2) + p*(pitch - pitchin) + i*ipitch + d*dpitch - yp*zdps - yd*dyaw);
}

void simplePD() {
  speeds[0] = HoverThrottle2 - p*(roll - rollin) - d*xdps;
  speeds[1] = HoverThrottle2 - p*(rollin - roll) + d*xdps;
  speeds[2] = HoverThrottle2 - p*(pitch - pitchin) - d*ydps;
  speeds[3] = HoverThrottle2 - p*(pitchin - pitch) + d*ydps;
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

void SetTuningsNoOvershoot() {
 p = 0.2 * p;
 i = 2.2 * p / period;
 d = p * period / 8.0;
}
 
 
