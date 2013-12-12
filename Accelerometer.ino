void getAccelValues() {
  sensors_event_t aevent;
  accel.getEvent(&aevent);
  
  ay = aevent.acceleration.x;
  ax = aevent.acceleration.y;
  az = -1 * aevent.acceleration.z;

  ar = sqrt(ax*ax + ay*ay + az*az);
  
  thetaX = -1 * (90.0 - (acos(ax/ar)*(180.0/PI)));
  thetaY = 90.0 - (acos(ay/ar)*(180.0/PI));
  thetaZ = acos(az/ar)*(180.0/PI);
  
  ax2 = ax/sqrt(2) - ay/sqrt(2);
  ay2 = ax/sqrt(2) + ay/sqrt(2);
  ar2 = sqrt(ax2*ax2 + ay2*ay2 + az*az);
  
  thetaX2 = -1 * (90.0 - (acos(ax2/ar2)*(180.0/PI)));
  thetaY2 = 90.0 - (acos(ay2/ar2)*(180.0/PI));
}

long pitchsum, rollsum;
float LevelRoll, LevelPitch;

void AccelLevelCalibration() {
  unsigned long loops;
  pitchsum = 0;
  rollsum = 0;
  MillisTracker = millis();
  while(millis() < MillisTracker + MilliCalibrationTime) {
    pitchsum += pitch;
    rollsum += roll;
    loops++;
  }
  LevelPitch = pitchsum / loops;
  LevelRoll = rollsum / loops;
}   
