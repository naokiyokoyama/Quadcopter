//Variables

void ComplementaryFilter() {
  oldpitch = pitch;
  oldroll  = roll;
  pitch = (CFRatio)*(pitch + gtrap[1])+(1.0 - CFRatio)*(theta[1]);
  roll  = (CFRatio)*(roll  + gtrap[0])+(1.0 - CFRatio)*(theta[0]);
}

void transform(float *array, int startelement) {
  float hold = array[startelement+1];
  array[startelement+1] = array[startelement]/sqrt2 - array[startelement+1]/sqrt2;
  array[startelement] = -1 * (array[startelement]/sqrt2 + hold/sqrt2); 
}
