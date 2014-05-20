
const int MaxWave = 254;        // Upper speed limit
const int MinWave = ESCMin;         // Lower speed limit

float tune;

float tuning(int low, int high) {
  float constant;
  
  int rough = map(volatileRx[1], 1050, 2000, low, high);
  constant = float(rough) / 1000.0;
  if(constant < 0) constant = 0.0;
  tune = constant;
  return constant;
}

//float period = 1.372;

void initPID(){
  rangle.changeConstants(aP, aI, aD, ainthresh, aoutthresh);
  pangle.changeConstants(aP, aI, aD, ainthresh, aoutthresh);
  rrate.changeConstants (rP, rI, rD, rinthresh, routthresh);
  prate.changeConstants (rP, rI, rD, rinthresh, routthresh);
  yrate.changeConstants (yP, yI, yD, yinthresh, youtthresh);
}

void throttleCtrl() {
  // Control the throttle with the right vertical
  throttle = map(trueRx[1], 0, 1010, ESCMin, MaxWave);
  if(throttle < ESCMin) throttle = ESCMin;
}

void flightControl() {
  float timePassed = MicrosPassed * 0.000001;
  int rollPID, pitchPID, yawPID;
  setZ = map(trueRx[0], -500, 500, -125, 125);
  yawPID = (int)yrate.Compute(dps[2] + (float)setZ, timePassed, 0);
  transformint(trueRx, 2);
 
  if(RightToggle) {
    setX=-map(trueRx[2], -500, 500, -45, 45);
    setY=-map(trueRx[3], -500, 500, -45, 45);
    
    float rerror, perror;
    rerror = roll  - (float)setX;
    perror = pitch - (float)setY;
    
    float rD, pD;
    if(abs(rerror) > 2) 
      rD = 0.087;
    else
      rD = aD;
    if(abs(perror) > 2) 
      pD = 0.087;
    else
      pD = aD;
      
    rangle.changeConstants(aP, aI, rD, ainthresh, aoutthresh);
    pangle.changeConstants(aP, aI, pD, ainthresh, aoutthresh);
    
    rollPID  = (int)rangle.Compute(rerror, timePassed, dps[0]);
    pitchPID = (int)pangle.Compute(perror, timePassed, dps[1]);
  }
  else {
    setX=map(trueRx[2], -500, 500, -20*3, 20*3);
    setY=map(trueRx[3], -500, 500, -20*3, 20*3);
    rollPID  = (int)rrate.Compute(dps[0] + (float)setX, timePassed, 0);
    pitchPID = (int)prate.Compute(dps[1] + (float)setY, timePassed, 0);
  }
  implementSpeeds(rollPID, pitchPID, yawPID);
}

void implementSpeeds(int RollPID, int PitchPID, int YawPID) {
  int array[4];
  array[0] = throttle - RollPID  - YawPID;  // East 
  array[1] = throttle + RollPID  - YawPID;  // West 
  array[2] = throttle - PitchPID + YawPID;  // North
  array[3] = throttle + PitchPID + YawPID;  // South
  
  int diff = 0;
  for(int x=0; x<4; x++) {
    if((array[x] - MaxWave) > diff) {
      diff = array[x] - MaxWave;
    }
  }
  
  for(int x=0; x<4; x++) {
    array[x] -= diff;
    if(array[x] < MinWave)
      array[x] = MinWave;
  }
  
  analogWrite(East,  array[0]);
  analogWrite(West,  array[1]);
  analogWrite(North, array[2]);
  analogWrite(South, array[3]);
}

//void inCtrl() {
//  rollin  = -2 * map(trueRx[2], -505, 505, -15, 15);
//  pitchin = -2 * map(trueRx[3], -505, 505, -15, 15);
//}
