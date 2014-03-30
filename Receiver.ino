//VARIABLES

// Interrupt Pins
const int Ch1 = 5;                 // Pin 18, right horizontal
const int Ch2 = 4;                 // Pin 19, right vertical
const int Ch3 = 0;                 // Pin 2, left horizontal
const int Ch4 = 1;                 // Pin 3, left vertical

volatile unsigned long CalibrateRxTracker, riseCh1, riseCh2, riseCh3, riseCh4; 

// Average Idle Channel Outputs

int zeroRx[4];

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
  static long sumRx[4];
  
  if(calibrate && millis() < CalibrateRxTracker + 1000 && millis() > CalibrateRxTracker + 300) {    
    for(int x=0; x<4; x++) 
      {sumRx[x] += volatileRx[x];}
    cloops++;
  } 
  else if(calibrate && millis() > CalibrateRxTracker + 400) {
    for(int x=0; x<4; x++) 
      {zeroRx[x] = sumRx[x] / cloops;}
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
      
      int pulse2 = pulseIn(Ch6, HIGH);
      if(pulse2 > 1700) 
      RightToggle = true;
      else if (pulse2 < 1300)
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
  volatileRx[0] = micros() - riseCh1;
}
  
void risingCh2Signal() {
  attachInterrupt(Ch2, fallingCh2Signal, FALLING);
  riseCh2 = micros();
}

void fallingCh2Signal() {
  attachInterrupt(Ch2, risingCh2Signal, RISING);
  volatileRx[1] = micros() - riseCh2;
}

void risingCh3Signal() {
  attachInterrupt(Ch3, fallingCh3Signal, FALLING);
  riseCh3 = micros();
}

void fallingCh3Signal() {
  attachInterrupt(Ch3, risingCh3Signal, RISING);
  volatileRx[2] = micros() - riseCh3;
}

void risingCh4Signal() {
  attachInterrupt(Ch4, fallingCh4Signal, FALLING);
  riseCh4 = micros();
}

void fallingCh4Signal() {
  attachInterrupt(Ch4, risingCh4Signal, RISING);
  volatileRx[3] = micros() - riseCh4;
}

void getRxValues() {
  static const int threshold = 30;
  
  if(calibrated) {
    for(int x=0; x<4; x++) {
      trueRx[x] = volatileRx[x] - zeroRx[x];
      if(abs(trueRx[x]) < threshold)
        trueRx[x] = 0;
    }
  }
}

// SETUP FUNCTIONS

void setupReceiverInterrupts() {
  attachInterrupt(Ch1, startInterrupts, RISING);
}


