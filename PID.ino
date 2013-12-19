
int MaxWave = 110;
int HoverThrottle = MaxWave - 50;

double DesiredPitch = 0.0;
double DesiredRoll = 0.0;

double NSComplement, WEComplement;

//double NWKpSoft = 0.5;//.4
//double NWKiSoft = 0.0;
//double NWKdSoft = 0.035;//.037
//
//double NWKpMedium = 0.9;//.4
//double NWKiMedium = 0.0;
//double NWKdMedium = 0.046;//.037
//
//double NWKpHard = 1.111;//1.111
//double NWKiHard = 0.0;
//double NWKdHard = 0.06;//.04
//
//double Kp2 = 0.25;//.4
//double Ki2 = 0.0;
//double Kd2 = 0.0;//.037

double NWKpSoft = 0.5;//.4
double NWKiSoft = 0.0;
double NWKdSoft = 0.035;//.037

double NWKpMedium = 0.9;//.4
double NWKiMedium = 0.0;
double NWKdMedium = 0.046;//.037

double NWKpHard = 1.111;//1.111
double NWKiHard = 0.0;
double NWKdHard = 0.06;//.04

double Kp2 = 0.25;//.4
double Ki2 = 0.0;
double Kd2 = 0.0;//.037

PID NSPID(&pitch, &NSComplement, &DesiredPitch, NWKpHard, NWKiHard, NWKdHard, REVERSE);
PID WEPID(&roll, &WEComplement, &DesiredRoll, Kp2, Ki2, Kd2, REVERSE);

void setupPIDs() {
  NSPID.SetMode(AUTOMATIC);
  WEPID.SetMode(AUTOMATIC);
  NSPID.SetSampleTime(5);
  WEPID.SetSampleTime(5);
  NSPID.SetOutputLimits((MaxWave - HoverThrottle) / -2, (MaxWave - HoverThrottle) / 2);
  WEPID.SetOutputLimits((MaxWave - HoverThrottle) / -2, (MaxWave - HoverThrottle) / 2);
}

double NwT, SwT, SeT, NeT;

void computePIDS() {
  NSPID.Compute();
  WEPID.Compute();
}

void Hover() {
//  if(abs(pitch) > 25) NSPID.SetTunings(NWKpHard, NWKiHard, NWKdHard);
//  else if(abs(pitch) > 10) NSPID.SetTunings(NWKpMedium, NWKiMedium, NWKdMedium);
//  else 
  NSPID.SetTunings(NWKpSoft, NWKiSoft, NWKdSoft);
  computePIDS();
  North.write(HoverThrottle - NSComplement + (MaxWave - HoverThrottle) / 2);
  //North.write(110);
  //East.write(HoverThrottle - WEComplement + (MaxWave - HoverThrottle) / 2);
  East.write(10);
  South.write(HoverThrottle + NSComplement + (MaxWave - HoverThrottle) / 2);
  //South.write(110);
  //West.write(HoverThrottle + WEComplement + (MaxWave - HoverThrottle) / 2);
  West.write(10);
}

void checkPairRatio() {
  //North.write(MaxWave);South.write(HoverThrottle);
  North.write(95);South.write(75);
}
  
