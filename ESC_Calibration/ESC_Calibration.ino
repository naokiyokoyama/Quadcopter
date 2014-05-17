int ESCMin = 124;
int ESCMax = 254;

const int North = 3;
const int West  = 4;
const int South = 5;
const int East  = 6;

void setup() {
  pinMode(North, OUTPUT);
  pinMode(West,  OUTPUT);
  pinMode(South, OUTPUT);
  pinMode(East,  OUTPUT);
  while(millis() < 4000) {
    analogWrite(North, ESCMax);
    analogWrite(West,  ESCMax);
    analogWrite(South, ESCMax);
    analogWrite(East,  ESCMax);
  }
  while(1 == 1) {
    analogWrite(North, ESCMin);
    analogWrite(West,  ESCMin);
    analogWrite(South, ESCMin);
    analogWrite(East,  ESCMin);
  }
}

void loop() {}
