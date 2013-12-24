//Variables

void ComplementaryFilter() {
  oldpitch = pitch;
  oldroll = roll;
  pitch = (CFRatio)*(pitch + gy)+(1.0 - CFRatio)*(thetaY);
  roll = (CFRatio)*(roll + gx)+(1.0 - CFRatio)*(thetaX);
}

