#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Wire.h>
#include <PID_v1.h>
#include <Servo.h>

// Assign a unique ID to the Adafruit sensors
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

// Variables created to measure time passed
unsigned long MicrosPassed;         // Amount of time taken to run the last loop
unsigned long MicrosTracker;        // Value in which the current time is stored to compute the time passed
unsigned long MillisTracker;        // Value in which the current time is stored to compute the time passed
unsigned long LoopTracker;          // Value in which the current time is stored to compute the time passed
unsigned int LoopTime;              // Time the loop takes in milliseconds

boolean printing = false;

// ESC 
boolean STOP = false;               // When this boolean is false, the motors permanently stop running

Servo North;                        // Northwest
Servo West;                         // Southwest
Servo South;                        // Southeast
Servo East;                         // Northeast

const int ESCMin = 10; // Minimum pulse that will be sent to the ESC

// GYROSCOPE
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

const int GyroAddress = 105;

long xRmSum, yRmSum, zRmSum, ZRLCloops;  // Variables used to calibrate the gyroscope 

float xdps, ydps, zdps;                  // Degrees per second calculated for every axis
float xdps1, ydps1, zdps1;               // DPS from the last loop to be used for the Trapezoidal Rule
float gx, gy, gz;                        // Intgrated angle values

float SC = 0.069657;                   // Scale factor in dps/LSB
int xRm, yRm, zRm;                       // Raw measurements from the gyroscope
int xRo = 1;                       // Zero rate level angles, or byte values when there is no angular velocity present
int yRo = -1;
int zRo = -2;
int xRth = 20;                           // Threshold for gyroscope byte values to reduce ambient noise
int yRth = 20;
int zRth = 20; 

// Variables used to calibrate the zero rate level angles

int MilliCalibrationTime = 200;        // Time provided for calibration (Gyro, receiver) in milliseconds   

// ACCELEROMETER 
int AccelAddress = 25; 

double ax, ay, az, ar, thetaX, thetaY, thetaZ; 
double fax, fay, faz;

// MAGNETOMETER

double mx, my, mz;
double heading;

// ANGLES
const double CFRatio = 0.98;
const double YawRatio = 0.8;
double pitch, roll, yaw;
double oldpitch, oldroll, oldyaw; 
double LevelPitch, LevelRoll;
double pitchin, rollin, yawin;

// RECEIVER
boolean LeftToggle, RightToggle;
volatile int RightHorizontalVolatile, LeftVerticalVolatile, RightVerticalVolatile, LeftHorizontalVolatile;

//NOISE REDUCTION
const int samples = 18;
double axRm[samples];  // Contains past values in order to filter out noise, with [0] being the most recent
double ayRm[samples];
double azRm[samples];
double axsum, aysum, azsum;
int axindex, ayindex, azindex;


void setup() {
  Wire.begin();
  Serial.begin(9600); printing = true;
  setupMotors();                              // Attaches each motor to its appropriate pin and sends them the minimum wave
  while(!accel.begin() || !mag.begin()){}     // Waits until the LSM303DLHC is found
  delay(300);                                 // Makes sure that the sensors have been properly initialized
  itgWrite(itgAddress, DLPF_FS, (DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_1)); //Sets the gyroscope scale for the outputs to +/-2000 degrees per second
  itgWrite(itgAddress, SMPLRT_DIV, 9);        // Sets the sample rate to 100 hz
//  ZeroRateLevelCalibration();                // Creates xRo, yRo, zRo
  getAccelValues();                           // Uses the accelerometer to calculate the inital angles before movement
  ar = sqrt(ax*ax + ay*ay + az*az);
  roll = 90.0 - (acos(ax/ar)*(180.0/PI));
  pitch = 90.0 - (acos(ay/ar)*(180.0/PI));
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
    getAccelAngles(ax, ay, az);
    ComplementaryFilter();                    // Low pass filter on the accelerometer, high pass filter on the gyroscope
    getMagValues();
    getMagHeading();
    getYaw();
    printValues();
    MicrosPassed = micros() - MicrosTracker;  // Saves the time it took for the loop
  }
}

void loop() {  
  LoopTime = millis() - LoopTracker;  LoopTracker = millis();
  MicrosTracker = micros(); 
  calibrateRxLoop();
  getGyroValues();                          // Computes Rm for each axis                     
  logOldDPS();                              // Saves the DPS values from the last loop before they are updated, for the Trapezoidal Rule
  getGyroDPS(xRm, yRm, zRm);                // Converts Rm values into dps (multiply by SC, threshold filtering, drift compensation)
  gx = DeltaTrapezoidalRule(xdps1, xdps);
  gy = DeltaTrapezoidalRule(ydps1, ydps);
  gz = DeltaTrapezoidalRule(zdps1, zdps);
  getAccelValues();                         // Computes acceleration in each axis
  fax = simpleMovingAverage(ax, axsum, axRm);
  fay = simpleMovingAverage(ay, aysum, ayRm);
  faz = simpleMovingAverage(az, azsum, azRm);
  getAccelAngles(fax, fay, faz);
  ComplementaryFilter();                    // Low pass filter on the accelerometer, high pass filter on the gyroscope
  getMagValues();
  getMagHeading();
  getYaw();
  ESCFunctions(); 
  printValues();
  MicrosPassed = micros() - MicrosTracker;  // Saves the time it took for the loop
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

float DeltaTrapezoidalRule(float old, float current) { // Approximate integration using the Trapezoidal Rule
  float trapezoid = (old + current) * (float)MicrosPassed * 0.0000005;
  return trapezoid; 
}




