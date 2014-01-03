


void ZeroRateLevelCalibration() {
  MillisTracker = millis();
  while(millis() < MillisTracker + MilliCalibrationTime) {
    getGyroValues();
    xRmSum += xRm;
    yRmSum += yRm;
    zRmSum += zRm;
    ZRLCloops++;
  }
  xRo = xRmSum / ZRLCloops;
  yRo = yRmSum / ZRLCloops;
  zRo = zRmSum / ZRLCloops;
}   

void getGyroValues() {
  xRm = -readY();
  yRm = readX();
  zRm = -readZ();
}

void getGyroDPS(int XRM, int YRM, int ZRM) {
  int dR;
  if(abs(XRM - xRo) > xRth) {
    dR = XRM - xRo;
    xdps = SC * (float)dR - (float)xRo;
  }
  else
  {xdps = 0;}
  
  if(abs(YRM - yRo) > yRth) {
    dR = YRM - yRo;
    ydps = SC * (float)dR - (float)yRo;
  }
  else
  {ydps = 0;}
  
  if(abs(ZRM - zRo) > zRth) {
    dR = ZRM - zRo;
    zdps = SC * (float)dR -  - (float)zRo;
  }
  else
  {zdps = 0;}
}

void transformGyroDPS() {
  int oldydps = ydps;
  ydps = xdps/sqrt(2) - ydps/sqrt(2);
  xdps = -1 * (xdps/sqrt(2) + oldydps/sqrt(2));  
}

void logOldDPS() { 
  xdps1 = xdps;
  ydps1 = ydps;
  zdps1 = zdps;
}

void getGyroAngles() {
  gx = xdps * (float)MicrosPassed / 1000000.0;
  gy = ydps * (float)MicrosPassed / 1000000.0;
  gz = zdps * (float)MicrosPassed / 1000000.0;
}

//This function will write a value to a register on the itg-3200.
//Parameters:
//  char address: The I2C address of the sensor. For the ITG-3200 breakout the address is 0x69.
//  char registerAddress: The address of the register on the sensor that should be written to.
//  char data: The value to be written to the specified register.
void itgWrite(char address, char registerAddress, char data)
{
  //Initiate a communication sequence with the desired i2c device
  Wire.beginTransmission(address);
  //Tell the I2C address which register we are writing to
  Wire.write(registerAddress);
  //Send the value to write to the specified register
  Wire.write(data);
  //End the communication sequence
  Wire.endTransmission();
}

//This function will read the data from a specified register on the ITG-3200 and return the value.
//Parameters:
//  char address: The I2C address of the sensor. For the ITG-3200 breakout the address is 0x69.
//  char registerAddress: The address of the register on the sensor that should be read
//Return:
//  unsigned char: The value currently residing in the specified register
unsigned char itgRead(char address, char registerAddress)
{
  //This variable will hold the contents read from the i2c device.
  unsigned char data=0;
  
  //Send the register address to be read.
  Wire.beginTransmission(address);
  //Send the Register Address
  Wire.write(registerAddress);
  //End the communication sequence.
  Wire.endTransmission();
  
  //Ask the I2C device for data
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 1);
  
  //Wait for a response from the I2C device
  if(Wire.available()){
    //Save the data sent from the I2C device
    data = Wire.read();
  }
  
  //End the communication sequence.
  Wire.endTransmission();
  
  //Return the data read during the operation
  return data;
}

int readX(void)
{
  int data=0;
  data = itgRead(itgAddress, GYRO_XOUT_H)<<8;
  data |= itgRead(itgAddress, GYRO_XOUT_L);  
  
  return data;
}

int readY(void)
{
  int data=0;
  data = itgRead(itgAddress, GYRO_YOUT_H)<<8;
  data |= itgRead(itgAddress, GYRO_YOUT_L);  
  
  return data;
}

int readZ(void)
{
  int data=0;
  data = itgRead(itgAddress, GYRO_ZOUT_H)<<8;
  data |= itgRead(itgAddress, GYRO_ZOUT_L);  
  
  return data;
}



