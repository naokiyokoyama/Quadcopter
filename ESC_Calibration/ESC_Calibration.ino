#include <Servo.h>

Servo Nw; // Northwest
Servo Sw; // Southwest
Servo Se; // Southeast
Servo Ne; // Northeast

int ESCMin = 10;

void setup() {
  Nw.attach(7);
  Sw.attach(6);
  Se.attach(5);
  Ne.attach(4);
  while(analogRead(15) < 4) {
    Nw.write(179);
    Sw.write(179);
    Se.write(179);
    Ne.write(179);
  }
  while(1 == 1) {
    Nw.write(ESCMin);
    Sw.write(ESCMin);
    Se.write(ESCMin);
    Ne.write(ESCMin);
  }
}

void loop() {
  // put your main code here, to run repeatedly: 
  
}
