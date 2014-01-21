#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

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

float xRmSum, yRmSum, zRmSum, ZRLCloops;  // Variables used to calibrate the gyroscope 

double xdps, ydps, zdps;                  // Degrees per second calculated for every axis
double xdps1, ydps1, zdps1;               // DPS from the last loop to be used for the Trapezoidal Rule
double gx, gy, gz;                        // Intgrated angle values

double SC = 0.069657;                   // Scale factor in dps/LSB
int xRm, yRm, zRm;                       // Raw measurements from the gyroscope
double xRo = 0.008;                       // Zero rate level angles, or byte values when there is no angular velocity present
double yRo = -0.128;
double zRo = -0.153;
int xRth = 20;                           // Threshold for gyroscope byte values to reduce ambient noise
int yRth = 20;
int zRth = 20; 


#define SCALE 2  // accel full-scale, should be 2, 4, or 8
 
/* LSM303 Address definitions */
#define LSM303_MAG  (0x3C >> 1)  // assuming SA0 grounded
#define LSM303_ACC  (0x32 >> 1)  // assuming SA0 grounded
 
/* LSM303 Register definitions */
#define CTRL_REG1_A 0x20
#define CTRL_REG2_A 0x21
#define CTRL_REG3_A 0x22
#define CTRL_REG4_A 0x23
#define CTRL_REG5_A 0x24
#define HP_FILTER_RESET_A 0x25
#define REFERENCE_A 0x26
#define STATUS_REG_A 0x27
#define OUT_X_L_A 0x28
#define OUT_X_H_A 0x29
#define OUT_Y_L_A 0x2A
#define OUT_Y_H_A 0x2B
#define OUT_Z_L_A 0x2C
#define OUT_Z_H_A 0x2D
#define INT1_CFG_A 0x30
#define INT1_SOURCE_A 0x31
#define INT1_THS_A 0x32
#define INT1_DURATION_A 0x33
#define CRA_REG_M 0x00
#define CRB_REG_M 0x01
#define MR_REG_M 0x02
#define OUT_X_H_M 0x03
#define OUT_X_L_M 0x04
#define OUT_Y_H_M 0x05
#define OUT_Y_L_M 0x06
#define OUT_Z_H_M 0x07
#define OUT_Z_L_M 0x08
#define SR_REG_M 0x09
#define IRA_REG_M 0x0A
#define IRB_REG_M 0x0B
#define IRC_REG_M 0x0C


void itgWrite(char address, char registerAddress, char data)
{
  Wire.beginTransmission(address);
  Wire.write(registerAddress);
  Wire.write(data);
  Wire.endTransmission();
}

unsigned char itgRead(char address, char registerAddress)
{
  unsigned char data=0;
  Wire.beginTransmission(address);
  Wire.write(registerAddress);
  Wire.endTransmission();
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 1);
  while(!Wire.available());
  data = Wire.read();
  Wire.endTransmission();
  return data;
}

byte LSM303_read(byte address)
{
  byte temp;
   
  if (address >= 0x20)
    Wire.beginTransmission(LSM303_ACC);
  else
    Wire.beginTransmission(LSM303_MAG);
     
  Wire.write(address);
   
  if (address >= 0x20)
    Wire.requestFrom(LSM303_ACC, 1);
  else
    Wire.requestFrom(LSM303_MAG, 1);
  while(!Wire.available())
    ;
  temp = Wire.read();
  Wire.endTransmission();
   
  return temp;
}
 
void LSM303_write(byte data, byte address)
{
  if (address >= 0x20)
    Wire.beginTransmission(LSM303_ACC);
  else
    Wire.beginTransmission(LSM303_MAG);
     
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}

// Variables used to calibrate the zero rate level angles

int MilliCalibrationTime = 16000;        // Time provided for calibration (Gyro, receiver) in milliseconds   

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
double LevelPitch, LevelRoll;
double pitchin, rollin, yawin;

// RECEIVER
boolean LeftToggle, RightToggle;
volatile int RightHorizontalVolatile, LeftVerticalVolatile, RightVerticalVolatile, LeftHorizontalVolatile;

//NOISE REDUCTION
const int samples = 20;
double axRm[samples];  // Contains past values in order to filter out noise, with [0] being the most recent
double ayRm[samples];
double azRm[samples];
double axsum, aysum, azsum;

double simpleMovingAverage(double raw, double total, double *array) {
  for(int x=(samples - 1); x>0; x--) {
    array[x] = array[x-1];
  }
  
  array[0] = raw;
  
  total = 0.0;
  
  for(int x=0; x<samples; x++) {
    total += array[x];
  }
  
  return total / samples;
}

float DeltaTrapezoidalRule(float old, float current) { // Approximate integration using the Trapezoidal Rule
  float trapezoid = (old + current) * (float)MicrosPassed * 0.0000005;
  return trapezoid; 
}

