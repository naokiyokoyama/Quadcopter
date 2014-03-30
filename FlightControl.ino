
int speeds[4];

int MaxWave = 254;        // Upper speed limit
int MinWave = ESCMin;         // Lower speed limit
int throttle = ESCMin;  // Speed of motor when level

boolean positionMode = true;

// Rate PID gains
double Rp = .25;
double Ri = .95;
double Rd = .011;//.01397 * .9;
// Position PID gains
double Pp = 5.00;// = 0.03; 
double Pi = .02;//; = 0.02;
double Pd = -.015;//.197;

void initPID(){
  pitchRatePID.changeConstants(Rp, Ri, Rd);
  rollRatePID.changeConstants (Rp, Ri, Rd);
  pitchPositionPID.changeConstants(Pp, Pi, Pd);
  rollPositionPID.changeConstants (Pp, Pi, Pd);
}
double printt;
void flightControl() {
  // Control the throttle with the right side
  throttle = map(trueRx[1], 0, 1010, 150, 245);
  if(throttle < 145) throttle = 145;

//  if(RightToggle) {
//    double k = double(map(volatileRx[1], 990, 2012, 000, 200)) / 1000.0;
//    if(k < 0) k = .00;
//    printt = k;
//    pitchRatePID.changeConstants(k, 0.0, 0.0);
//    rollRatePID.changeConstants (k, 0.0, 0.0);
//  }
//  else 
//    throttle = map(volatileRx[1], 990, 2012, 150, 245);
//  if(throttle < 145) throttle = 145;
//  
//  throttle = 195;

  timePassed = MicrosPassed * 0.000001;
  
  if (positionMode == true){
    int X = map(trueRx[3], -500, 500, -15, 15);
    int Y = map(trueRx[2], -500, 500, -15, 15);
    rollin  = pitchPositionPID.Compute(roll  - (double)X, timePassed, xdps);
    pitchin = rollPositionPID.Compute (pitch - (double)Y, timePassed, ydps);
  }
  else {
    rollin  = double(map(trueRx[2], -500, 500, -45, 45));
    pitchin = double(map(trueRx[3], -500, 500, -45, 45));
  }

  int rollPID  = (int)pitchRatePID.Compute(xdps - rollin,  timePassed);
  int pitchPID = (int)rollRatePID.Compute (ydps - pitchin, timePassed);
  
  speeds[0] = throttle + rollPID;
  speeds[1] = throttle - rollPID;
  speeds[2] = throttle + pitchPID;
  speeds[3] = throttle - pitchPID;
  
  // Implement speed limits
  for(int x=0; x<4; x++) {
    speeds[x] = constrain(speeds[x], MinWave, MaxWave);
  }
  
  analogWrite(East,  speeds[0]);
  analogWrite(West,  speeds[1]);
  analogWrite(North, speeds[2]);
  analogWrite(South, speeds[3]);
}
