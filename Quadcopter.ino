#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Wire.h>
#include <PID_v1.h>
#include <Servo.h>
double pitch2;
// Assign a unique ID to the Adafruit sensors
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

unsigned long MicrosPassed;
unsigned long MicrosPassedPresent;
unsigned long MicrosTracker;
unsigned long MillisTracker;
unsigned long LoopTracker;
unsigned int LoopTime;

// ESC ////////////////////////////////////////////////////////////////////////////////////////////////////////////
boolean STOP = false;

Servo North; // Northwest
Servo West; // Southwest
Servo South; // Southeast
Servo East; // Northeast

const int ESCMin = 10; // Minimum pulse that will be sent to the ESC

// GYROSCOPE //////////////////////////////////////////////////////////////////////////////////////////////////////
char WHO_AM_I = 0x00;
char SMPLRT_DIV= 0x15;
char DLPF_FS = 0x16;
char GYRO_XOUT_H = 0x1D;
char GYRO_XOUT_L = 0x1E;
char GYRO_YOUT_H = 0x1F;
char GYRO_YOUT_L = 0x20;
char GYRO_ZOUT_H = 0x21;
char GYRO_ZOUT_L = 0x22;

char DLPF_CFG_0 = (1<<0);
char DLPF_CFG_1 = (1<<1);
char DLPF_CFG_2 = (1<<2);
char DLPF_FS_SEL_0 = (1<<3);
char DLPF_FS_SEL_1 = (1<<4);

char itgAddress = 0x69;

int GyroAddress = 105;

long xRmSum, yRmSum, zRmSum, ZRLCloops;

float xdps, ydps, zdps;                // Degrees per second calculated for every axis
float xdps1, ydps1, zdps1;
float gx, gy, gz;                      // Intgrated angle values
    
float SC = 1.0/14.375;                 // Scale factor in dps/LSB
int xRm, yRm, zRm;
int xRm1, yRm1, zRm1, xRm2, yRm2, zRm2; 
int xRo, yRo, zRo;                     // Zero rate level angles, or byte values when there is no angular velocity present
int xRth = 20;                         // Threshold for gyroscope byte values to reduce ambient noise
int yRth = 20;
int zRth = 20; 

// Variables used to calibrate the zero rate level angles

int MilliCalibrationTime = 100;        // Time provided for calibration (Gyro, receiver) in milliseconds   
  
// ACCELEROMETER //////////////////////////////////////////////////////////////////////////////////////////////////
 
#define CTRL_REG2_A 0x21
 
int AccelAddress = 25; 
 
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
  setupMotors();
  while(!accel.begin() || !mag.begin()){}
  delay(300);
  writeI2C(AccelAddress, CTRL_REG2_A, 0b01111011);
  itgWrite(itgAddress, DLPF_FS, (DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_0)); //Set the gyroscope scale for the outputs to +/-2000 degrees per second
  itgWrite(itgAddress, SMPLRT_DIV, 9);       // Set the sample rate to 100 hz
  ZeroRateLevelCalibration();                // Creates xRo, yRo, zRo
  CalibrateIdleReceiverValues();
  while(millis() < 6000) {}
  MicrosTracker = micros();
}

void loop() {  
  //LoopTracker = millis();
  MicrosTracker = micros(); 
  
  receiveTransmitterSignals();              // Uses the volatile interupt values to read each joystick (-500 ~ 500)
  //logGyroNoise();                         // Saves the Rm values from the last loop before they are updated
  getGyroValues();                          // Computes Rm for each axis                     
  //reduceGyroNoise();                      // Filters radical noise from Rm to Rm2
  logOldDPS();                              // Saves the DPS values from the last loop before they are updated for the Trapezoidal Rule
  getGyroDPS(xRm, yRm, zRm);                // Converts nRm values into ndps (multiply by SC, threshold filtering)
  transformGyroDPS();                       // Rotates the mathematical model by 45 degrees
  getDeltaGyroAnglesTrapezoidalRule();      // Gets the change in tilt using approximate integration with the time taken for the previous loop via Trapezoidal Rule
  getAccelValues();                         // Computes acceleration and tilt in each axis
  ComplementaryFilter();                    // Low pass filter on the accelerometer, high pass filter on the gyroscope
  
  ESCFunctions(); 
  //printValues();
  
  MicrosPassed = micros() - MicrosTracker;  // Saves the time it took for the loop
  //LoopTime = millis() - LoopTracker;
}  
void ESCFunctions() {
  if(!STOP) {
    Hover();
    //checkPairRatio();
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



