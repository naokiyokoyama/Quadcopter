int StopPoints;                              // Points are incremented whenever a pivot point is reached 
const int StopTarget = 4;                    // Points needed to stop the quadcopter
unsigned long StopTracker;                   // Keeps track of the time interval between pivots
const int MilliStopperTime = 500;            // Maximum time in milliseconds alotted for pivots
int PivotSpeedThreshold = 200;               // Speed that needs to be reached to define a pivot
boolean StopBegun = false;                   // Indicates whether the pivot sequence has been initiated
int lastPivot;                               // Contains the last zdps value of the sequence while it still exists

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
  if(!LeftToggle) {STOP = true;}
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


