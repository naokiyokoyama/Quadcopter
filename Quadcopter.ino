#include <Wire.h>
#include <Servo.h>
#include "Config.h"

void setup() {
  Wire.begin();
  Serial.begin(9600); printing = true;
  delay(300);                                 // Makes sure that the sensors have been properly initialized
  initLSM303(SCALE);
  itgWrite(itgAddress, DLPF_FS, (DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_1)); //Sets the gyroscope scale for the outputs to +/-2000 degrees per second
  itgWrite(itgAddress, SMPLRT_DIV, 10);        // Sets the sample rate to 1kHz
//  ZeroRateLevelCalibration();                // Creates xRo, yRo, zRo
  setupMotors();                              // Attaches each motor to its appropriate pin and sends them the minimum wave
  getAccelValues();                           // Uses the accelerometer to calculate the inital angles before movement
  getAccelValues();                           // Uses the accelerometer to calculate the inital angles before movement
  getAccelAngles(ax, ay, az);
  pitch = thetaY;
  roll = thetaX;
  getMagValues();
  getMagHeading();
  yaw = heading;
  yawin = yaw;
  setupReceiverInterrupts();
  while(millis() < 6000) {
    calibrateRxLoop();
    MicrosTracker = micros(); 
    getGyroValues();                          // Computes Rm for each axis                     
    logOldDPS();                              // Saves the Dps values from the last loop before they are updated, for the Trapezoidal Rule
    getGyroDPS(xRm, yRm, zRm);                // Converts Rm values into dps (multiply by SC, threshold filtering)
    gx = DeltaTrapezoidalRule(xdps1, xdps);
    gy = DeltaTrapezoidalRule(ydps1, ydps);
    gz = DeltaTrapezoidalRule(zdps1, zdps);
    getAccelValues();                         // Computes acceleration and tilt in each axis
    fax = simpleMovingAverage(ax, axsum, axRm);
    fay = simpleMovingAverage(ay, aysum, ayRm);
    faz = simpleMovingAverage(az, azsum, azRm);
    getAccelAngles(fax, fay, faz);
    ComplementaryFilter();                    // Low pass filter on the accelerometer, high pass filter on the gyroscope
    getMagValues();
    getMagHeading();
    getYaw();
    printValues();
    MicrosPassed = micros() - MicrosTracker;  // Saves the time it took for the loop
  }
}

void loop() {  
  MicrosTracker = micros(); 
  calibrateRxLoop();
  getGyroValues();                          // Computes Rm for each axis                     
  logOldDPS();                              // Saves the DPS values from the last loop before they are updated, for the Trapezoidal Rule
  getGyroDPS(xRm, yRm, zRm);                // Converts Rm values into dps (multiply by SC, drift compensation)
  gx = DeltaTrapezoidalRule(xdps1, xdps);
  gy = DeltaTrapezoidalRule(ydps1, ydps);
  gz = DeltaTrapezoidalRule(zdps1, zdps);
  getAccelValues();                         // Computes acceleration in each axis
  fax = simpleMovingAverage(ax, axsum, axRm);
  fay = simpleMovingAverage(ay, aysum, ayRm);
  faz = simpleMovingAverage(az, azsum, azRm);
  getAccelAngles(fax, fay, faz);
  ComplementaryFilter();                   
  getMagValues();
  getMagHeading();
  getYaw();
  ESCFunctions();  
  printValues();
  MicrosPassed = micros() - MicrosTracker;  // Saves the time it took to run the loop
}  
void ESCFunctions() {
  if(!STOP) {
    MITPID();
//    Hover();
//    testSimple();
//    testDrive();
    triggerSTOP();
  }
  else {
    North.write(ESCMin);
    West.write(ESCMin);
    South.write(ESCMin);
    East.write(ESCMin);
    if(LeftToggle) {STOP = false;}
  }
}





