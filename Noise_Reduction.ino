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
