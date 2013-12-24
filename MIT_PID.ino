
int speeds[4];

int MaxWave2 = 179;        // Upper speed limit
int MinWave2 = 40;         // Lower speed limit
int HoverThrottle2 = 100;  // Speed of motor when level

double rollin = 0.0;
double pitchin = 0.0;//-48.4;

double p = 0.256; //.36
double i = 0.7;//0.9527812;
double d = 0.07;
double period = 1.379;

double droll, iroll, dpitch, ipitch;

void MITPID() {
  if(LeftVertical() > 250) {
   if(LeftVertical() > 400) pitchin = 30.0;
   else pitchin = 15.0; 
  }
  else if (LeftVertical() < -250) {
   if(LeftVertical() < -400) pitchin = -30.0;
   else pitchin = -15.0; 
  }
  else pitchin = LevelPitch;

  rollin = 0.0;
  pitchin = 0.0;
  
  if(abs(pitch - pitchin) < 1.3) pitch = pitchin;
  if(abs(roll - rollin) < 1.3) roll = rollin;
  
  HoverThrottle2 = RightVertical() * 14 / 100;
  
  if(HoverThrottle2>120) HoverThrottle2=120;
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

void simplePD() {
  speeds[0] = HoverThrottle2 - p*(roll - rollin) - d*xdps;
  speeds[1] = HoverThrottle2 - p*(rollin - roll) + d*xdps;
  speeds[2] = HoverThrottle2 - p*(pitch - pitchin) - d*ydps;
  speeds[3] = HoverThrottle2 - p*(pitchin - pitch) + d*ydps;
}

void deltaPID() {
//  droll = (roll - oldroll)/ ( (double) MicrosPassed / 1000000.0);
//  dpitch = (pitch - oldpitch)/ ( (double) MicrosPassed / 1000000.0);
  
  droll = xdps;
  dpitch = ydps;
  
  if(pitchin != pitch){
    iroll += DeltaTrapezoidalRule( oldroll - rollin, roll - rollin);
  }
  if(rollin !=roll){
    ipitch += DeltaTrapezoidalRule( oldpitch - pitchin, pitch - pitchin);
  }
  
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

void SetTuningsNoOvershoot() {
 p = 0.2 * p;
 i = 2.2 * p / period;
 d = p * period / 8.0;
}
 
 
