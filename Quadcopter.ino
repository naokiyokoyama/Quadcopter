#include <SMAFilter.h>
#include <IMU10DOF.h>
#include <PIDCont.h>
#include <Wire.h>
#include "Config.h"

SMAFilter accelFilter, gyroFilter, magFilter;
IMU10DOF IMU;
PIDCont rangle, pangle, rrate, prate, yrate; 

void initFilters() {
  accelFilter.init(20);
  gyroFilter.init(10);
}

// PID gains
//float aP = 0.2;
//float aD = 0.063; //79
//float aI = 0.67;
float aP = 0.67;
float aD = 0.0; //79
float aI = 0.07;
float ainthresh = 40.0;
float aoutthresh = 10.0;

//float rP = 0.25;
//float rD = 0.0; //79
//float rI = 0.0950;
float rP = 0.25;
float rD = 0.0; //79
float rI = 0.95;
float rinthresh  = 100.0;
float routthresh = 20.0;

float yP = 0.68;
float yD = 0.0; //79
float yI = 0.5;
float yinthresh  = 30.0;
float youtthresh = 40.0;

void setup() {
//  Serial.begin(9600); printing = true;
  initPID();                                  // Creates PID classes and assigns appropriate constants
  initFilters();
  IMU.init();
  delay(700);                                 // Makes sure that the sensors have been properly initialized
  rinse(100);
  ZeroRateLevelCalibration(false);            // Creates gyrozero
  setupMotors();                              // Attaches each motor to its appropriate pin and sends them the minimum wave
  setupReceiverInterrupts();
}

void loop() {  
  MicrosTracker = micros(); 
  calibrateRxLoop();
  getRxValues();
  
  int gyro[3];
  IMU.getGyroValues(gyro);                  // Computes Rm for each axis     
  float fgyro[3];
  gyroFilter.filter(gyro, fgyro);
  copyArray(olddps, dps);                              // Saves the DPS values from the last loop before they are updated, for the Trapezoidal Rule
  getGyroDPS(fgyro);                        // Converts Rm values into dps (multiply by SC, drift compensation)
  transform(dps, 0);
  driftCompensation(dps, gyrozero);
  for(int i=0;i<3;i++)
    gtrap[i] = DeltaTrapezoidalRule(olddps[i], dps[i]);
    
  int accel[3];
  IMU.getAccelValues(accel);                         // Computes acceleration in each axis
  float faccel[3];
  accelFilter.filter(accel, faccel);
  transform(faccel, 0);
  getAccelAngles(faccel);
  
  ComplementaryFilter();

  ESC();

  printValues();
  MicrosPassed = micros() - MicrosTracker;  // Saves the time it took to run the loop
}  

void ESC() {
  if(millis() > 6000 && !STOP) {
    throttleCtrl();
    flightControl();
    analogWrite(East, ESCMin);
    analogWrite(West, ESCMin);
    triggerSTOP();
  }
  else if(STOP) {
    rangle.resetI();
    pangle.resetI();
    rrate.resetI();
    prate.resetI();
    yrate.resetI();
    analogWrite(North, ESCMin);
    analogWrite(West,  ESCMin);
    analogWrite(South, ESCMin);
    analogWrite(East,  ESCMin);
    if(LeftToggle) STOP = false;
  }
}

void rinse(int time) {
  int accel[3], gyro[3], mag[3];
  unsigned long track = millis();
  while(millis() < track + time) {
    IMU.getAccelValues(accel);
    IMU.getGyroValues(gyro);
    IMU.getMagValues(mag);  
  }
}



