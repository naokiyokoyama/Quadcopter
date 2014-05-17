
void setupMotors() {
  pinMode(North, OUTPUT);
  pinMode(West,  OUTPUT);
  pinMode(South, OUTPUT);
  pinMode(East,  OUTPUT);
  analogWrite(North, ESCMin);
  analogWrite(West,  ESCMin);
  analogWrite(South, ESCMin);
  analogWrite(East,  ESCMin);
}

void testSimple() {
  analogWrite(North, 150);
  analogWrite(West,  150);
  analogWrite(South, 150);
  analogWrite(East,  150);
}

void testDrive() {
  int stickinput = map(trueRx[1], 0, 1010, 145, 240);
  
  analogWrite(North, stickinput);
  analogWrite(West,  stickinput);
  analogWrite(South, stickinput);
  analogWrite(East,  stickinput);
}

void triggerSTOP() {
  
  if(!LeftToggle) STOP = true;
  if(!controllerOn) STOP = true;
  
  static int StopPoints;                       // Points are incremented whenever a pivot point is reached 
  const int StopTarget = 4;                    // Points needed to stop the quadcopter
  static unsigned long StopTracker;            // Keeps track of the time interval between pivots
  const int MilliStopperTime = 500;            // Maximum time in milliseconds alotted for pivots before reset
  const int PivotSpeedThreshold = 200;         // Speed that needs to be reached to define a pivot
  static boolean StopBegun = false;            // Indicates whether the pivot sequence has been initiated
  static int lastPivot;                        // Contains the last zdps value of the sequence while it still exists
  
  if(StopPoints == StopTarget) {STOP = true;}
  if((dps[2] >= PivotSpeedThreshold || dps[2] <= -1 * PivotSpeedThreshold) && !StopBegun) {
    StopTracker = millis();
    lastPivot = dps[2];
    StopBegun = true;
    StopPoints = 1;
  }
  if((StopBegun && millis() - StopTracker < MilliStopperTime) && ((lastPivot < 0 && dps[2] > PivotSpeedThreshold) || (lastPivot > 0 && dps[2] < -1 * PivotSpeedThreshold))) {
    StopPoints++;
    lastPivot = dps[2];
    StopTracker = millis();
  }
  if(StopBegun && millis() - StopTracker >= MilliStopperTime) {
    StopBegun = false;
    StopPoints = 0;
  }
}


