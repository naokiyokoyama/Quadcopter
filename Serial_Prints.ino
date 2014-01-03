unsigned long PrintTracker;
unsigned int MilliPrintTime = 100; // Time between prints in milliseconds

void printValues() {
  if(millis() >= PrintTracker && printing) {
    PrintTracker = (millis() + MilliPrintTime);
    Serial.print(" yp:"); Serial.print(pyaw);
    Serial.print(" yi:"); Serial.print(iyaw);
    Serial.print(" yd:"); Serial.print(yd*zdps);
    Serial.println();
  }
}

void CompareRoRm() {
  Serial.print(" xRo:"); Serial.print(xRo);
  Serial.print(" yRo:"); Serial.print(yRo);
  Serial.print(" zRo:"); Serial.print(zRo);
  Serial.print(" xRm:"); Serial.print(xRm);
  Serial.print(" yRm:"); Serial.print(yRm);
  Serial.print(" zRm:"); Serial.print(zRm);
}

void IntegratedGyroValues() {
  Serial.print(" GyroPitch:"); Serial.print(gy);
  Serial.print(" GyroRoll:"); Serial.print(gx);
  Serial.print(" GyroYaw:"); Serial.print(gz);
}

void MilliLoopTime() {
  Serial.print(" LoopTime:"); Serial.print(LoopTime);
}

void VolatileControls() {
  Serial.print(" RightHorizontalVolatile:"); Serial.print(RightHorizontalVolatile);
  Serial.print(" LeftVerticalVolatile:"); Serial.print(LeftVerticalVolatile);
  Serial.print(" RightVerticalVolatile:"); Serial.print(RightVerticalVolatile);
  Serial.print(" LeftHorizontalVolatile:"); Serial.print(LeftHorizontalVolatile);

}

void Controls() {
  Serial.print(" RightHorizontal:"); Serial.print(RightHorizontal());
  Serial.print(" LeftVertical:"); Serial.print(LeftVertical());
  Serial.print(" RightVertical:"); Serial.print(RightVertical());
  Serial.print(" LeftHorizontal:"); Serial.print(LeftHorizontal());
  Serial.print(" LeftToggle:"); Serial.print(LeftToggle);
  Serial.print(" RightToggle:"); Serial.print(RightToggle);
}

void ZeroControls() {
  Serial.print(" RightHorizontalZero:"); Serial.print(RightHorizontalZero);
  Serial.print(" LeftVerticalZero:"); Serial.print(LeftVerticalZero);
  Serial.print(" RightVerticalZero:"); Serial.print(RightVerticalZero);
  Serial.print(" LeftHorizontalZero:"); Serial.print(LeftHorizontalZero);
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
