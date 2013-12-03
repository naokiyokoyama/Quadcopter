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
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24

int Addr = 105;                        // I2C Address of the gyroscope   
float xdps, ydps, zdps;                // Degrees per second calculated for every axis
float gx, gy, gz;                      // Intgrated angle values
    
float SC = .07;                        // Scale factor in dps/LSB
int xRm, yRm, zRm, dR;
int xRo, yRo, zRo;                     // Zero rate level angles, or byte values when there is no rotational velocity present
int xRth = 20;                         // Threshold for gyroscope byte values to reduce ambient noise
int yRth = 20;
int zRth = 20; 

// Variables used to calibrate the zero rate level angles
long xRsum, yRsum, zRsum;               // The sum that will be divided to produce the average
long C;                                 // The divisor, equal to the total amount of times the Rm has been read
int MilliCalibrationTime = 3000;        // Time provided for calibration (Gyro, receiver) in milliseconds   
  
// ACCELEROMETER //////////////////////////////////////////////////////////////////////////////////////////////////
 
float ax, ay, az, ar, thetaX, thetaY, thetaZ;  
float ax2, ay2, az2, ar2, thetaX2, thetaY2, thetaZ2; 

// KALMAN FILTER
double pitch, roll, yaw;

// RECEIVER
volatile int RightHorizontalVolatile, LeftVerticalVolatile, RightVerticalVolatile, LeftHorizontalVolatile;
int RightHorizontal, LeftVertical, RightVertical, LeftHorizontal;
volatile boolean LeftToggle, RightToggle;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  writeI2C(CTRL_REG1, 0b00001111);    // Turn on all axes, disable power down
  writeI2C(CTRL_REG2, 0b00000000);
  writeI2C(CTRL_REG3, 0b00001000);    // Enable control ready signal
  writeI2C(CTRL_REG4, 0b00110000);    // Set scale (2000 deg/sec)
  setupMotors();
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
  
  receiveTransmitterSignals();              // Uses the volatile interupt values to read each joystick (-500 ~ 500)
  logGyroNoise();                           // Shifts xRm/yRm/zRm logs one place towards the past
  getGyroValues();                          // Computes Rm for each axis
  getAccelValues();                         // Computes acceleration in each axis  
  MicrosPassed = micros() - MicrosTracker;  // Computes time taken for each loop 
  MicrosTracker = micros();                 
  logMicrosPassed();                        // Shifts all MicrosPassed values one place towards the past
  reduceGyroNoise();                        // Filters radical noise from Rm to Rm2
  getGyroDPS2();                            // Filtered-byte-to-DPS conversion with Rm2, ambient noise filtering
  transformGyroDPS();
  //getKalmanAngles();
  ComplementaryFilter();
  
  printValues();
  //ESCFunctions(); 
  
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



