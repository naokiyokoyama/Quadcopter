//VARIABLES

// Interrupt Pins
const int Ch1 = 5;                 // Pin 18, right horizontal
const int Ch2 = 4;                 // Pin 19, right vertical
const int Ch3 = 0;                 // Pin 2, left vertical
const int Ch4 = 1;                 // Pin 3, left horizontal

volatile unsigned long CalibrateRxTracker, riseCh1, riseCh2, riseCh3, riseCh4;
volatile int RightHorizontalVolatile, LeftVerticalVolatile, RightVerticalVolatile, LeftHorizontalVolatile;

int cloops;
unsigned long int ch1s, ch2s, ch3s, ch4s; 

// Average Idle Channel Outputs
int RightHorizontalZero, LeftVerticalZero, RightVerticalZero, LeftHorizontalZero;

// Digital Input Pins
const int Ch5 = 22;                 // Pin 2
const int Ch6 = 23;                 // Pin 3

boolean calibrate = false;
boolean calibrated = false;
boolean receiverBegun = false;
unsigned long ReceiverTracker;
int ReceiverSamplingTime = 700;
unsigned int Ch5Sum, Ch6Sum;
boolean lastLeftToggle = false;
boolean lastRightToggle = false;
boolean LeftToggleChanged = false;
boolean RightToggleChanged = false;
int LeftToggleSwitches, RightToggleSwitches;

// INTERRUPT FUNCTIONS (must be ISRs that do not accept any parameters)

void startInterrupts() {
  attachInterrupt(Ch1, risingCh1Signal, RISING);
  attachInterrupt(Ch2, risingCh2Signal, RISING);
  attachInterrupt(Ch3, risingCh3Signal, RISING);
  attachInterrupt(Ch4, risingCh4Signal, RISING);
  CalibrateRxTracker = millis();
  calibrate = true;
  ch1s = 0;
  ch2s = 0;
  ch3s = 0;
  ch4s = 0;
  cloops = 0;
}

void calibrateRxLoop() { 
  if(calibrate && millis() < CalibrateRxTracker + 1000) {
    ch1s += RightHorizontalVolatile;
    ch2s += RightVerticalVolatile;
    ch3s += LeftVerticalVolatile;
    ch4s += LeftHorizontalVolatile;
    cloops++;
  } 
  else if(calibrate) {
    RightHorizontalZero = ch1s/cloops;
    RightVerticalZero = ch2s/cloops;
    LeftVerticalZero = ch3s/cloops;
    LeftHorizontalZero = ch4s/cloops;
    calibrate = false;
    calibrated = true;
  }
}
void risingCh1Signal() {
  attachInterrupt(Ch1, fallingCh1Signal, FALLING);
  riseCh1 = micros();
}

void fallingCh1Signal() {
  attachInterrupt(Ch1, risingCh1Signal, RISING);
  RightHorizontalVolatile = micros() - riseCh1;
}
  
void risingCh2Signal() {
  attachInterrupt(Ch2, fallingCh2Signal, FALLING);
  riseCh2 = micros();
}

void fallingCh2Signal() {
  attachInterrupt(Ch2, risingCh2Signal, RISING);
  RightVerticalVolatile = micros() - riseCh2;
}

void risingCh3Signal() {
  attachInterrupt(Ch3, fallingCh3Signal, FALLING);
  riseCh3 = micros();
}

void fallingCh3Signal() {
  attachInterrupt(Ch3, risingCh3Signal, RISING);
  LeftVerticalVolatile = micros() - riseCh3;
}

void risingCh4Signal() {
  attachInterrupt(Ch4, fallingCh4Signal, FALLING);
  riseCh4 = micros();
}

void fallingCh4Signal() {
  attachInterrupt(Ch4, risingCh4Signal, RISING);
  LeftHorizontalVolatile = micros() - riseCh4;
}

int RightHorizontal() { 
  if(!calibrated) return 0;
  else return RightHorizontalVolatile - RightHorizontalZero;
}

int RightVertical() { 
  if(!calibrated) return 0;
  else return RightVerticalVolatile - RightVerticalZero;
}

int LeftVertical() { 
  if(!calibrated) return 0;
  else return LeftVerticalVolatile - LeftVerticalZero;
}

int LeftHorizontal() { 
  if(!calibrated) return 0;
  else return LeftHorizontalVolatile - LeftHorizontalZero;
}

// SETUP FUNCTIONS

void setupReceiverInterrupts() {
  attachInterrupt(Ch1, startInterrupts, RISING);
}

void CalibrateIdleReceiverValues() {
  unsigned long int IdleRxTracker;
  int IdleRxTime = 100;
  IdleRxTracker = millis();
  while(millis() < IdleRxTracker + IdleRxTime) {
    ch1s += RightHorizontalVolatile;
    ch2s += RightVerticalVolatile;
    ch3s += LeftVerticalVolatile;
    ch4s += LeftHorizontalVolatile;
    cloops++;
  }
  RightHorizontalZero = ch1s/cloops;
  RightVerticalZero = ch2s/cloops;
  LeftVerticalZero = ch3s/cloops;
  LeftHorizontalZero = ch4s/cloops;
}

void CalibrateIdleReceiverValues2() {
  RightHorizontalZero = RightHorizontalVolatile;
  LeftVerticalZero = LeftVerticalVolatile;
  RightVerticalZero = RightVerticalVolatile;
  LeftHorizontalZero = LeftHorizontalVolatile;
}

// MAIN LOOP FUNCTIONS

void receiveCh5Ch6Signal() {
  if(!receiverBegun) {
    receiverBegun = true;
    ReceiverTracker = millis();
  }
  if(receiverBegun) {
    if (millis() < ReceiverTracker + ReceiverSamplingTime) {
      Ch5Sum += digitalRead(Ch5);
      Ch6Sum += digitalRead(Ch6);
    }
    else {
      //leftwave = Ch5Sum; rightwave = Ch6Sum;
      if(Ch5Sum < 9)
      {LeftToggle = false;}
      else {LeftToggle = true;}
      if(Ch6Sum < 9)
      {RightToggle = false;}
      else {RightToggle = true;}
      Ch5Sum = 0;
      Ch6Sum = 0;
      receiverBegun = false;
    }
  }
}

void countToggleSwitching() {
  if(lastLeftToggle != LeftToggle)
  {LeftToggleChanged = true; LeftToggleSwitches++;}
  else {LeftToggleChanged = false;}
  if(lastRightToggle != RightToggle)
  {RightToggleChanged = true; RightToggleSwitches++;}
  else {RightToggleChanged = false;}
  lastLeftToggle = LeftToggle;
  lastRightToggle = RightToggle;
}
