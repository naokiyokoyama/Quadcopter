// Interrupt Pins
const int Ch1 = 5;                 // Pin 18
const int Ch2 = 4;                 // Pin 19
const int Ch3 = 0;                 // Pin 2
const int Ch4 = 1;                 // Pin 3
volatile unsigned long riseCh1, fallCh1, riseCh2, fallCh2, riseCh3, fallCh3, riseCh4, fallCh4;

// INTERRUPT FUNCTIONS

void risingCh1Signal() {
  attachInterrupt(Ch1, fallingCh1Signal, FALLING);
  riseCh1 = micros();
}

void fallingCh1Signal() {
  attachInterrupt(Ch1, risingCh1Signal, RISING);
  fallCh1 = micros();
  RightHorizontalVolatile = fallCh1 - riseCh1;
}
  
void risingCh2Signal() {
  attachInterrupt(Ch2, fallingCh2Signal, FALLING);
  riseCh2 = micros();
}

void fallingCh2Signal() {
  attachInterrupt(Ch2, risingCh2Signal, RISING);
  fallCh2 = micros();
  RightVerticalVolatile = fallCh2 - riseCh2;
}

void risingCh3Signal() {
  attachInterrupt(Ch3, fallingCh3Signal, FALLING);
  riseCh3 = micros();
}

void fallingCh3Signal() {
  attachInterrupt(Ch3, risingCh3Signal, RISING);
  fallCh3 = micros();
  LeftVerticalVolatile = fallCh3 - riseCh3;
}

void risingCh4Signal() {
  attachInterrupt(Ch4, fallingCh4Signal, FALLING);
  riseCh4 = micros();
}

void fallingCh4Signal() {
  attachInterrupt(Ch4, risingCh4Signal, RISING);
  fallCh4 = micros();
  LeftHorizontalVolatile = fallCh4 - riseCh4;
}



// Digital Input Pins
const int Ch5 = 22;                 // Pin 2
const int Ch6 = 23;                 // Pin 3

// Average Idle Channel Outputs
int RightHorizontalZero, LeftVerticalZero, RightVerticalZero, LeftHorizontalZero;


boolean receiverBegun = false;
unsigned long ReceiverTracker;
int ReceiverSamplingTime = 700;
unsigned int Ch5Sum, Ch6Sum;
boolean lastLeftToggle = false;
boolean lastRightToggle = false;
boolean LeftToggleChanged = false;
boolean RightToggleChanged = false;
int LeftToggleSwitches, RightToggleSwitches;

// SETUP FUNCTIONS
void CalibrateIdleReceiverValues() {
  unsigned long int IdleRxTracker;
  int IdleRxTime= 100;
  int loops;
  unsigned long int ch1s, ch2s, ch3s, ch4s; 
  IdleRxTracker = millis();
  while(millis() < IdleRxTracker + IdleRxTime) {
    ch1s += RightHorizontalVolatile;
    ch2s += RightVerticalVolatile;
    ch3s += LeftVerticalVolatile;
    ch4s += LeftHorizontalVolatile;
    loops++;
  }
  RightHorizontalZero = ch1s/loops;
  RightVerticalZero = ch2s/loops;
  LeftVerticalZero = ch3s/loops;
  LeftHorizontalZero = ch4s/loops;
}

void CalibrateIdleReceiverValues2() {
  RightHorizontalZero = RightHorizontalVolatile;
  LeftVerticalZero = LeftVerticalVolatile;
  RightVerticalZero = RightVerticalVolatile;
  LeftHorizontalZero = LeftHorizontalVolatile;
}

void setupReceiverInterrupts() {
  attachInterrupt(Ch1, risingCh1Signal, RISING);
  attachInterrupt(Ch2, risingCh2Signal, RISING);
  attachInterrupt(Ch3, risingCh3Signal, RISING);
  attachInterrupt(Ch4, risingCh4Signal, RISING);
}





// MAIN LOOP FUNCTIONS

void receiveTransmitterSignals() {
  RightHorizontal = RightHorizontalVolatile - RightHorizontalZero;
  if(RightHorizontal > 500)
  {RightHorizontal = 500;}
  else if(RightHorizontal < -500)
  {RightHorizontal = -500;}
  LeftVertical = LeftVerticalVolatile - LeftVerticalZero;
  if(LeftVertical > 500)
  {LeftVertical = 500;}
  else if(LeftVertical < -500)
  {LeftVertical = -500;}
  RightVertical = RightVerticalVolatile - RightVerticalZero;
  if(RightVertical > 500)
  {RightVertical = 500;}
  else if(RightVertical < -500)
  {RightVertical = -500;}
  LeftHorizontal = LeftHorizontalVolatile - LeftHorizontalZero;
  if(LeftHorizontal > 500)
  {LeftHorizontal = 500;}
  else if(LeftHorizontal < -500)
  {LeftHorizontal = -500;}
  receiveCh5Ch6Signal();
  countToggleSwitching();
}

//int leftwave, rightwave;

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
