
int speeds[4];

int MaxWave2 = 179;        // Upper speed limit
int MinWave2 = 10;         // Lower speed limit
int HoverThrottle2 = 100;  // Speed of motor when level

//double p, i, d;
double p = 1.5;
double i = 0.852;//0.099;//0.9527812;
double d = 0.42;

double yp = .00;// = 0.03; 
double yi = .00;//; = 0.02;
double yd = .00;//.197;

double period = 1.379;
double proll, iroll, ppitch, ipitch, pyaw, iyaw;
double integralThreshold = 6.0;

void MITPID() {
  // Control the pitch and roll with the left side of the radio
  pitchin = double(map(LeftVertical(), -500, 500, -15, 15) * 2);
  rollin = double(map(LeftHorizontal(), -500, 500, -15, 15) * 2);
  // Control the throttle with the right side
  HoverThrottle2 = map(RightVertical(), 0, 1010, 40, 150);
  if(HoverThrottle2 < 30) HoverThrottle2 = 30;
  
  deltaPID();
  
  // Speed limits
  for(int x=0; x<4; x++) {
    if(speeds[x] > MaxWave2) speeds[x] = MaxWave2;
    if(speeds[x] < MinWave2) speeds[x] = MinWave2;
  }
  
  East.write(speeds[0]);
  West.write(speeds[1]);
  North.write(speeds[2]);
  South.write(speeds[3]);
}

void deltaPID() {
  // Save pyaw before it's updated
  double oldpyaw = pyaw;
  double oldproll = proll;
  double oldppitch = ppitch;
  
  if(abs(yawin - yaw) > 180.0) {
    double yawin2 = yawin + 180.0;
    double yaw2 = yaw + 180.0;
    if(yawin2 > 360.0) yawin2 -= 360.0;
    if(yaw2 > 360.0) yaw2 -= 360.0;
    pyaw = -(yawin2 - yaw2);
  }
  else 
    pyaw = -(yawin - yaw);
  
  proll = roll - rollin;
  ppitch = pitch - pitchin;
  
//  yd = double(map(RightVerticalVolatile, 990, 2012, 130, 170)) / 100.0;
//  if(yd < 0) yd = .001;
//  HoverThrottle2 = 90;
  
  iyaw += DeltaTrapezoidalRule(oldpyaw, pyaw);
  iroll += DeltaTrapezoidalRule(oldproll, proll);
  ipitch += DeltaTrapezoidalRule(oldppitch, ppitch);
  
  if(iroll > integralThreshold) iroll = integralThreshold;
  else if(iroll < -integralThreshold) iroll = -integralThreshold;
  if(ipitch > integralThreshold) ipitch = integralThreshold;
  else if(ipitch < -integralThreshold) ipitch = -integralThreshold;
  
  speeds[0] = int(double(HoverThrottle2) - p*proll - i*iroll - d*xdps + yp*pyaw + yi*iyaw + yd*zdps); 
  speeds[1] = int(double(HoverThrottle2) + p*proll + i*iroll + d*xdps + yp*pyaw + yi*iyaw + yd*zdps); 
  speeds[2] = int(double(HoverThrottle2) - p*ppitch - i*ipitch - d*ydps - yp*pyaw - yi*iyaw - yd*zdps);
  speeds[3] = int(double(HoverThrottle2) + p*ppitch + i*ipitch + d*ydps - yp*pyaw - yi*iyaw - yd*zdps);
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
 
 
