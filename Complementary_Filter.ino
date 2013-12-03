//Variables

void ComplementaryFilter() {
  pitch = (.98)*(pitch + gy)+(0.02)*(thetaY2);
  roll = (.98)*(roll + gx)+(0.02)*(thetaX2);
}
