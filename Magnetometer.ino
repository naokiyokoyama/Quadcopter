const float mxoffset = -10.91;
const float myoffset = 14.865;
const float mzoffset = 4.035;


void getMagValues() {
  sensors_event_t mevent;
  mag.getEvent(&mevent);
  
  mx = mevent.magnetic.x + mxoffset; 
  my = mevent.magnetic.y + myoffset;
  mz = -1 * (mevent.magnetic.z + mzoffset);
}

void getMagHeading() {
  double newpitch = roll * 0.017453;
  double newroll = pitch * 0.017453;
  
  double truemx = mx * cos(newpitch) + mz * sin(newpitch);
  double truemy = mx * sin(newroll) * sin(newpitch) + my * cos(newroll) - mz * sin(newroll) * cos(newpitch);
  
  heading = atan2(truemy, truemx) * 57.29577;
  if(heading < 0) heading += 360.0;
}

void getYaw(){
  double gyroyaw = yaw + gz;
  
  if(gyroyaw < 0.0) gyroyaw += 360.0;
  if(gyroyaw > 360.0) gyroyaw -= 360; 
  
  yaw = (YawRatio) * (gyroyaw) + (1.0 - YawRatio) * (heading); 
  
  if(abs(gyroyaw - heading) > 180) 
  yaw = gyroyaw;
}
