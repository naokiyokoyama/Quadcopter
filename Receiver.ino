
// 1: 1243 2: 912 3: 1435 4: 1438 Neutral
// 1: 725  2: 919 3: 939  4: 930  Low


// VARIABLES

// Interrupt Pins
const int Ch1 = 12;                 // Pin 18, right horizontal 12
const int Ch2 = 11;                 // Pin 19, right vertical 11
const int Ch3 = 15;                 // Pin 2, left horizontal 15
const int Ch4 = 10;                 // Pin 3, left vertical 10
const int Ch5 = 13;                 // Pin 2 13
const int Ch6 = 14;                 // Pin 3 14

volatile unsigned long CalibrateRxTracker, riseCh1, riseCh2, riseCh3, riseCh4, riseCh5, riseCh6; 

// Average Idle Channel Outputs

int zeroRx[4];

boolean calibrate = false;
boolean calibrated = false;

// INTERRUPT FUNCTIONS (must be ISRs that do not accept any parameters)

void startInterrupts() {
  attachInterrupt(Ch1, risingCh1Signal, RISING);
  attachInterrupt(Ch2, risingCh2Signal, RISING);
  attachInterrupt(Ch3, risingCh3Signal, RISING);
  attachInterrupt(Ch4, risingCh4Signal, RISING);
  attachInterrupt(Ch5, risingCh5Signal, RISING);
  attachInterrupt(Ch6, risingCh6Signal, RISING);
  CalibrateRxTracker = millis();
  calibrate = true;
}

void calibrateRxLoop() { 
  static unsigned int cloops;
  static long sumRx[4];
  
  if(calibrate && millis() < CalibrateRxTracker + 1000 && millis() > CalibrateRxTracker + 300) {    
    for(int x=0; x<4; x++) 
      sumRx[x] += volatileRx[x];
    cloops++;
  } 
  else if(calibrate && millis() > CalibrateRxTracker + 400) {
    for(int x=0; x<4; x++) 
      zeroRx[x] = sumRx[x] / cloops;
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

void risingCh5Signal() {
  attachInterrupt(Ch5, fallingCh5Signal, FALLING);
  riseCh5 = micros();
}

void fallingCh5Signal() {
  attachInterrupt(Ch5, risingCh5Signal, RISING);
  volatileRx[4] = micros() - riseCh5;
}

void risingCh6Signal() {
  attachInterrupt(Ch6, fallingCh6Signal, FALLING);
  riseCh6 = micros();
}

void fallingCh6Signal() {
  attachInterrupt(Ch6, risingCh6Signal, RISING);
  volatileRx[5] = micros() - riseCh6;
}

void getRxValues() {
  static const int threshold = 30;
  
  if(calibrated) {
    for(int x=0; x<4; x++) {
      trueRx[x] = volatileRx[x] - zeroRx[x];
      if(abs(trueRx[x]) < threshold)
        trueRx[x] = 0;
    }
    controllerOn = true;
    if(volatileRx[4] > 1750) LeftToggle = true;
    else if(volatileRx[4] < 1250) LeftToggle = false;
    else controllerOn = false;
    if(volatileRx[5] > 1750) RightToggle = true;
    else RightToggle = false;
  }
}

// SETUP FUNCTIONS

void setupReceiverInterrupts() {
  pinMode(Ch1, INPUT);
  pinMode(Ch2, INPUT);
  pinMode(Ch3, INPUT);
  pinMode(Ch4, INPUT);
  pinMode(Ch5, INPUT);
  pinMode(Ch6, INPUT);
  attachInterrupt(Ch1, startInterrupts, RISING);
}


