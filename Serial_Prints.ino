unsigned long PrintTracker;
unsigned int MilliPrintTime = 100; // Time between prints in milliseconds

void printValues() {
  if(millis() >= PrintTracker && printing) {
    PrintTracker = (millis() + MilliPrintTime);
//    Serial.print(" xRo:"); Serial.print(printt, DEC);
    Serial.print(" pitchin:"); Serial.print(pitchin);
    Serial.print(" rollin:"); Serial.print(rollin);
    Serial.println();
  }
}

void CompareRoRm() {
  Serial.print(" xRo:"); Serial.print(xRo, DEC);
  Serial.print(" yRo:"); Serial.print(yRo, DEC);
  Serial.print(" zRo:"); Serial.print(zRo, DEC);
  Serial.print(" xRm:"); Serial.print(xRm);
  Serial.print(" yRm:"); Serial.print(yRm);
  Serial.print(" zRm:"); Serial.print(zRm);
}

void RxValues() {
  for(int x=0; x<4; x++) {
    Serial.print(trueRx[x]);
    Serial.print("\t\t"); 
  }
}

void motorValues() {
  for(int x=0; x<4; x++) {
    Serial.print(speeds[x]);
    Serial.print("\t\t"); 
  }
}

void IntegratedGyroValues() {
  Serial.print(" GyroPitch:"); Serial.print(gy);
  Serial.print(" GyroRoll:"); Serial.print(gx);
  Serial.print(" GyroYaw:"); Serial.print(gz);
}

void MilliLoopTime() {
  Serial.print(" LoopTime:"); Serial.print(LoopTime);
}

void LevelAccelTilts() {
  Serial.print(" LevelPitch:"); Serial.print(LevelPitch);
  Serial.print(" LevelRoll:"); Serial.print(LevelRoll);
}

//void PIDSerovOutputs() {
//  Serial.print(" NwT:"); Serial.print(NwT);
//  Serial.print(" SwT:"); Serial.print(SwT);
//  Serial.print(" SeT:"); Serial.print(SeT);
//  Serial.print(" NeT:"); Serial.print(NeT);
//}

void DPS() {
  Serial.print(" xdps:"); Serial.print(xdps);
  Serial.print(" ydps:"); Serial.print(ydps);
  Serial.print(" zdps:"); Serial.print(zdps);
  //Serial.print(" R:"); Serial.print(Reduced);
}

void PitchRoll() {
  Serial.print(" pitch:"); Serial.print(pitch);
  Serial.print(" roll:"); Serial.print(roll);
}

void MATLABGraph() {
  Serial.print("       "); Serial.print(thetaX);
  Serial.print("       "); Serial.print(0);
  Serial.print("       "); Serial.print(0);
  Serial.print("       "); Serial.print(millis());
}

  
void Accel() {
  Serial.print(" ax:"); Serial.print(ax);
  Serial.print(" ay:"); Serial.print(ay);
  Serial.print(" az:"); Serial.print(az);
}

void AccelandGyro() {
  Serial.print("  gx:"); Serial.print(gx);
  Serial.print("  thetaX:"); Serial.print(thetaX);
  Serial.print("  gy:"); Serial.print(gy);
  Serial.print("  thetaY:"); Serial.print(thetaY);
  Serial.print("  gz:"); Serial.print(gz);
  Serial.print("  thetaZ:"); Serial.print(thetaZ);
  Serial.print(" pitch:"); Serial.print(pitch);
  Serial.print("     roll:"); Serial.print(roll);
}

void Theta() {
  Serial.print(" thetaX:"); Serial.print(thetaX);
  Serial.print(" thetaY:"); Serial.print(thetaY);
  Serial.print(" thetaZ:"); Serial.print(thetaZ);
}

//void pidvalues() {
//  Serial.print(" pitch:"); Serial.print(pitch);
//  Serial.print("     NSComplement:"); Serial.print(NSComplement);
//  Serial.print("     roll:"); Serial.print(roll);
//  Serial.print("     WEComplement:"); Serial.print(WEComplement);
//}
