int HoverThrottle = 30;
int MaxWave = 100;

double DesiredPitch = 0.0;
double DesiredRoll = 0.0;

double NSComplement, WEComplement;

double Kp = .15;//.4
double Ki = 0.01;
double Kd = 0.05;//.037

double Kpsoft = 0.25;//.4
double Kisoft = 0.0;
double Kdsoft = .0;//.037

//if (abs(pitch - DesiredPitch) > 35) {
  //NSPID.SetTunings( , , );

PID NSPID(&pitch, &NSComplement, &DesiredPitch, Kp, Ki, Kd, REVERSE);
PID WEPID(&roll, &WEComplement, &DesiredRoll, Kp, Ki, Kd, REVERSE);


void setupPIDs() {
  NSPID.SetMode(AUTOMATIC);
  WEPID.SetMode(AUTOMATIC);
  NSPID.SetSampleTime(5);
  WEPID.SetSampleTime(5);
  NSPID.SetOutputLimits(HoverThrottle, MaxWave);
  WEPID.SetOutputLimits(HoverThrottle, MaxWave);
}

double NwT, SwT, SeT, NeT;

void computePIDS() {
  NSPID.Compute();
  WEPID.Compute();
}

void Hover() {
  computePIDS();
  NorthMotor.write(HoverThrottle - NSComplement + (MaxWave - HoverThrottle) / 2);
  //Sw.write(HoverThrottle + WEComplement + (MaxWave - HoverThrottle) / 2);
  WestMotor.write(10);
  SouthMotor.write(HoverThrottle + NSComplement + (MaxWave - HoverThrottle) / 2);
  //Ne.write(HoverThrottle - WEComplement + (MaxWave - HoverThrottle) / 2);
  EastMotor.write(10);
}
