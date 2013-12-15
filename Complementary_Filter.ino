//Variables

void ComplementaryFilter() {
  oldpitch = pitch;
  oldroll = roll;
  pitch = (.98)*(pitch + gy)+(0.02)*(thetaY2);
  roll = (.98)*(roll + gx)+(0.02)*(thetaX2);
}
