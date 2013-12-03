int StopPoints;                              // Points are incremented whenever a pivot point is reached 
const int StopTarget = 4;                    // Points needed to stop the quadcopter
unsigned long StopTracker;                   // Keeps track of the time interval between pivots
const int MilliStopperTime = 500;            // Maximum time in milliseconds alotted for pivots
int PivotSpeedThreshold = 200;               // Speed that needs to be reached to define a pivot
boolean StopBegun = false;                   // Indicates whether the pivot sequence has been initiated
int lastPivot;                               // Contains the last zdps value of the sequence while it still exists

void setupMotors() {
  NorthMotor.attach(7);
  WestMotor.attach(6);
  SouthMotor.attach(5);
  EastMotor.attach(4);
  NorthMotor.write(ESCMin);
  WestMotor.write(ESCMin);
  SouthMotor.write(ESCMin);
  EastMotor.write(ESCMin);
}

void testSimple() {
  NorthMotor.write(100);
  WestMotor.write(100);
  SouthMotor.write(100);
  EastMotor.write(100);
}

void triggerSTOP() {
  if(LeftVertical > 480) {STOP = true;}
  //if(thetaZ < 10.0) {STOP = true;}
  if(StopPoints == StopTarget) {STOP = true;}
  if((zdps >= PivotSpeedThreshold || zdps <= -1 * PivotSpeedThreshold) && !StopBegun) {
    StopTracker = millis();
    lastPivot = zdps;
    StopBegun = true;
    StopPoints = 1;
  }
  if((StopBegun && millis() - StopTracker < MilliStopperTime) && ((lastPivot < 0 && zdps > PivotSpeedThreshold) || (lastPivot > 0 && zdps < -1 * PivotSpeedThreshold))) {
    StopPoints++;
    lastPivot = zdps;
    StopTracker = millis();
  }
  if(StopBegun && millis() - StopTracker >= MilliStopperTime) {
    StopBegun = false;
    StopPoints = 0;
  }
}


