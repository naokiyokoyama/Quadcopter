
void printValues() {
  static unsigned long PrintTracker;
  const unsigned int MilliPrintTime = 300;  
  
  if(millis() >= PrintTracker && printing) {
    PrintTracker = (millis() + MilliPrintTime);
    RC();
    Serial.println();
  }
}

void Set() {
  Serial.print(" setX: "); Serial.print(setX); Serial.print("\t");
  Serial.print(" setY: "); Serial.print(setY); Serial.print("\t"); 
  Serial.print(" setZ: "); Serial.print(setZ); Serial.print("\t");
}

void MicrosLoop() {
  Serial.print(" microsloop: "); Serial.print(MicrosPassed); Serial.print("\t");
}

void RPin() {
  Serial.print(" pitchin: "); Serial.print(pitchin); Serial.print("\t");
  Serial.print(" rollin: ") ; Serial.print(rollin) ; Serial.print("\t"); 
}

void Tune() {
  Serial.print(" tune: "); Serial.print(tune, DEC); Serial.print("\t");
}

void RC() {
  Serial.print(" 1: "); Serial.print(trueRx[0]); Serial.print("\t");
  Serial.print(" 2: "); Serial.print(trueRx[1]); Serial.print("\t"); 
  Serial.print(" 3: "); Serial.print(trueRx[2]); Serial.print("\t");
  Serial.print(" 4: "); Serial.print(trueRx[3]); Serial.print("\t");
  Serial.print(" Left: "); Serial.print(LeftToggle); Serial.print("\t");  
  Serial.print(" Right: "); Serial.print(RightToggle); Serial.print("\t");
}

void Volatile() {
  Serial.print(" 1: "); Serial.print(volatileRx[0]); Serial.print("\t");
  Serial.print(" 2: "); Serial.print(volatileRx[1]); Serial.print("\t"); 
  Serial.print(" 3: "); Serial.print(volatileRx[2]); Serial.print("\t");
  Serial.print(" 4: "); Serial.print(volatileRx[3]); Serial.print("\t");
  Serial.print(" 5: "); Serial.print(volatileRx[4]); Serial.print("\t");  
  Serial.print(" 6: "); Serial.print(volatileRx[5]); Serial.print("\t");
}

void RollPitch() {
  Serial.print(" roll: ") ; Serial.print(roll, DEC) ; Serial.print("\t"); 
  Serial.print(" pitch: "); Serial.print(pitch, DEC); Serial.print("\t");
}

void Theta() {
  Serial.print(" thetaX: "); Serial.print(theta[0], DEC); Serial.print("\t");
  Serial.print(" thetaY: "); Serial.print(theta[1], DEC); Serial.print("\t"); 
  Serial.print(" thetaZ: "); Serial.print(theta[2], DEC); Serial.print("\t");
}

void Dps() {
  Serial.print(" dpsX: "); Serial.print(dps[0], DEC); Serial.print("\t");
  Serial.print(" dpsY: "); Serial.print(dps[1], DEC); Serial.print("\t");
  Serial.print(" dpsZ: "); Serial.print(dps[2], DEC); Serial.print("\t");
}

void Dps2() {
  Serial.print(" dps2X: "); Serial.print(dps2[0], DEC); Serial.print("\t");
  Serial.print(" dps2Y: "); Serial.print(dps2[1], DEC); Serial.print("\t");
  Serial.print(" dps2Z: "); Serial.print(dps2[2], DEC); Serial.print("\t");
}

void gyroZero() {
  Serial.print(" gyrozeroX: "); Serial.print(gyrozero[0], DEC);
  Serial.print(" gyrozeroY: "); Serial.print(gyrozero[1], DEC);  
  Serial.print(" gyrozeroZ: "); Serial.print(gyrozero[2], DEC);
}

void Heading() {
  Serial.print(" heading: "); Serial.print(heading);
}
