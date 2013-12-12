#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303.h>
#include <Wire.h>
#include <PID_v1.h>
#include <Servo.h>

// Assign a unique ID to the Adafruit sensors
Adafruit_LSM303_Accel accel = Adafruit_LSM303_Accel(54321);
Adafruit_LSM303_Mag mag = Adafruit_LSM303_Mag(12345);

unsigned long MicrosPassed;
unsigned long MicrosPassedPresent;
unsigned long MicrosTracker;
unsigned long MillisTracker;
unsigned long LoopTracker;
unsigned int LoopTime;

// ESC ////////////////////////////////////////////////////////////////////////////////////////////////////////////
boolean STOP;

Servo North; // Northwest
Servo West; // Southwest
Servo South; // Southeast
Servo East; // Northeast

const int ESCMin = 10; // Minimum pulse that will be sent to the ESC

// GYROSCOPE //////////////////////////////////////////////////////////////////////////////////////////////////////

float xdps, ydps, zdps;                // Degrees per second calculated for every axis
float gx, gy, gz;                      // Intgrated angle values
    
float SC = .07;                        // Scale factor in LSB/dps
int xRm, yRm, zRm, dR;
int xRo, yRo, zRo;                     // Zero rate level angles, or byte values when there is no rotational velocity present
int xRth = 20;                         // Threshold for gyroscope byte values to reduce ambient noise
int yRth = 20;
int zRth = 20; 

// Variables used to calibrate the zero rate level angles

int MilliCalibrationTime = 100;        // Time provided for calibration (Gyro, receiver) in milliseconds   
  
// ACCELEROMETER //////////////////////////////////////////////////////////////////////////////////////////////////
 
float ax, ay, az, ar, thetaX, thetaY, thetaZ;  
float ax2, ay2, az2, ar2, thetaX2, thetaY2, thetaZ2; // After transformation

// ANGLES
double pitch, roll, yaw;

// RECEIVER
volatile int RightHorizontalVolatile, LeftVerticalVolatile, RightVerticalVolatile, LeftHorizontalVolatile;
int RightHorizontal, LeftVertical, RightVertical, LeftHorizontal;
volatile boolean LeftToggle, RightToggle;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  setupPIDs();  
  setupReceiverInterrupts();
  while(!accel.begin() || !mag.begin()){}
  delay(300);
  CalibrateIdleReceiverValues2();
  ZeroRateLevelCalibration();
  while(millis() < 7000) {}
  MicrosTracker = micros();
}

void loop() {  
  //LoopTracker = millis();
  MicrosTracker = micros(); 
  
  receiveTransmitterSignals();              // Uses the volatile interupt values to read each joystick (-500 ~ 500)
  logGyroNoise();                           // Saves the Rm values from the last loop before they are updated
  getGyroValues();                          // Computes Rm for each axis
  getAccelValues();                         // Computes acceleration in each axis                     
  reduceGyroNoise();                        // Filters radical noise from Rm to Rm2
  logOldDPS();                              // Saves the DPS values from the last loop before they are updated
  getGyroDPS2();                            // Filtered-byte-to-DPS conversion with Rm2, ambient noise filtering
  transformGyroDPS();                       // Rotates the mathematical model by 45 degrees
  getDeltaGyroAnglesTrapezoidalRule();      // Gets the change in tilt using approximate integration with the Trapezoidal Rule and the time taken for the previous loop
  ComplementaryFilter();                    // Low pass filter on the accelerometer, high pass filter on the gyroscope
  
  //ESCFunctions(); 
  printValues();
  
  MicrosPassed = micros() - MicrosTracker;  // Saves the time it took for the loop
  //LoopTime = millis() - LoopTracker;
}  
void ESCFunctions() {
  if(!STOP) {
    Hover();
    //testSimple();
    triggerSTOP();
  }
  else {
    North.write(ESCMin);
    West.write(ESCMin);
    South.write(ESCMin);
    East.write(ESCMin);
    while(1 == 1) {}
  }
}



