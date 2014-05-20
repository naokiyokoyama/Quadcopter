#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

float pitchin, rollin, yawin;
int setX, setY, setZ;
int thetazero[3];

const float sqrt2 = 1.41421;

// Variables created to measure time passed
unsigned long MicrosPassed;         // Amount of time taken to run the last loop
unsigned long MicrosTracker;        // Value in which the current time is stored to compute the time passed

boolean printing = false;

// ESC 
boolean STOP = false;               // When this boolean is false, the motors permanently stop running

const int ESCMin = 124; // Minimum pulse that will be sent to the ESC
int throttle;

const int North = 3;
const int West  = 4;
const int South = 5;
const int East  = 6;

float gyrozero[3];
float dps[3];
float dps2[3];
float olddps[3];
float gtrap[3];
const float gyroSC = .07;

// ACCELEROMETER 
float theta[3];

// MAGNETOMETER
float heading;
int magoffsets[] = {95,-120,-124};

// ANGLES
const float YawRatio = 0.8;
float pitch, roll, yaw, oldpitch, oldroll, oldyaw;

// RECEIVER
boolean LeftToggle, RightToggle, controllerOn;
int trueRx[4];
volatile int volatileRx[6];

// NOISE REDUCTION
const int samples = 20;
int faccel2D[3][samples];
int accelsum[3];
int fgyro2D[3][samples];
int gyrosum[3];

