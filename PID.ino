//
//int speedsPID[4];
//
//int MaxWave = 179;
//int MinWave = 40;
//int HoverThrottle;
//
//double DesiredPitch;
//double DesiredRoll;
//
//double NSComplement, WEComplement;
//
//double p = 0.23;
//double i = 0.0;
//double d = 0.0;
//
//double period = 1.31;
//
//PID NSPID(&pitch, &NSComplement, &DesiredPitch, p, i, d, REVERSE);
//PID WEPID(&roll, &WEComplement, &DesiredRoll, p, i, d, REVERSE);
//
//void setupPIDs() {
//  NSPID.SetMode(AUTOMATIC);
//  WEPID.SetMode(AUTOMATIC);
//  NSPID.SetSampleTime(1);
//  WEPID.SetSampleTime(1);
//  NSPID.SetOutputLimits(-169, 169);
//  WEPID.SetOutputLimits(-169, 169);
//}
//
//void Hover() {
//  HoverThrottle = RightVertical() * 14 / 100;
//  
//  NSPID.SetTunings(p, i, d);
//  WEPID.SetTunings(p, i, d);
//  
//  NSPID.Compute();
//  WEPID.Compute();
//  
//  speedsPID[0] = HoverThrottle - (int)WEComplement;
//  speedsPID[1] = HoverThrottle + (int)WEComplement;
//  speedsPID[2] = HoverThrottle - (int)NSComplement;
//  speedsPID[3] = HoverThrottle + (int)NSComplement;
//  
//  for(int x=0; x<4; x++) {
//    if(speedsPID[x]>MaxWave) speedsPID[x]=MaxWave;
//    if(speedsPID[x]<MinWave) speedsPID[x]=MinWave;
//  }
//  
//  East.write(speedsPID[0]);
//  West.write(speedsPID[1]);
////  North.write(speedsPID[2]);
////  South.write(speedsPID[3]);
//}
//  
//void SetTuningsClassicPID() {
// p = 0.6 * p;
// i = 2.0 * p / period;
// d = p * period / 8.0;
//}
//
//void SetTuningsPessenIntegralRule() {
// p = 0.7 * p;
// i = 2.5 * p / period;
// d = p * period * 0.15;
//}
//
//void SetTuningsSomeOvershoot() {
// p = 0.33 * p;
// i = 2.0 * p / period;
// d = p * period / 3.0;
//}
//
//void SetTuningsNoOvershoot() {
// p = 0.2 * p;
// i = 2.2 * p / period;
//// d = p * period / 8.0;
//}
// 
// 
