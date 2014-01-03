//VARIABLES

// Interrupt Pins
const int Ch1 = 5;                 // Pin 18, right horizontal
const int Ch2 = 4;                 // Pin 19, right vertical
const int Ch3 = 0;                 // Pin 2, left vertical
const int Ch4 = 1;                 // Pin 3, left horizontal

volatile unsigned long CalibrateRxTracker, riseCh1, riseCh2, riseCh3, riseCh4; 

// Average Idle Channel Outputs
int RightHorizontalZero, LeftVerticalZero, RightVerticalZero, LeftHorizontalZero;

// Digital Input Pins
const int Ch5 = 22;                 // Pin 2
const int Ch6 = 23;                 // Pin 3

boolean calibrate = false;
boolean calibrated = false;

// INTERRUPT FUNCTIONS (must be ISRs that do not accept any parameters)

void startInterrupts() {
  attachInterrupt(Ch1, risingCh1Signal, RISING);
  attachInterrupt(Ch2, risingCh2Signal, RISING);
  attachInterrupt(Ch3, risingCh3Signal, RISING);
  attachInterrupt(Ch4, risingCh4Signal, RISING);
  CalibrateRxTracker = millis();
  calibrate = true;
}

void calibrateRxLoop() { 
  static unsigned int cloops;
  static long ch1s, ch2s, ch3s, ch4s;
  
  if(calibrate && millis() < CalibrateRxTracker + 1000 && millis() > CalibrateRxTracker + 300) {    
    ch1s += RightHorizontalVolatile;
    ch2s += RightVerticalVolatile;
    ch3s += LeftVerticalVolatile;
    ch4s += LeftHorizontalVolatile;
    cloops++;
  } 
  else if(calibrate && millis() > CalibrateRxTracker + 400) {
    RightHorizontalZero = ch1s/cloops;
    RightVerticalZero = ch2s/cloops;
    LeftVerticalZero = ch3s/cloops;
    LeftHorizontalZero = ch4s/cloops;
    calibrate = false;
    calibrated = true;
  }
  if(calibrated){
    static unsigned long RxTracker;
    if(millis() > RxTracker + 700){
      RxTracker = millis();
      int pulse = pulseIn(Ch5, HIGH);
      if(pulse > 1700) 
      LeftToggle = true;
      else if (pulse < 1300)
      LeftToggle = false; 
      
      pulse = pulseIn(Ch6, HIGH);
      if(pulse > 1700) 
      RightToggle = true;
      else if (pulse < 1300)
      RightToggle = false;
    }
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

void CalibrateIdleReceiverValues2() {
  RightHorizontalZero = RightHorizontalVolatile;
  LeftVerticalZero = LeftVerticalVolatile;
  RightVerticalZero = RightVerticalVolatile;
  LeftHorizontalZero = LeftHorizontalVolatile;
}


