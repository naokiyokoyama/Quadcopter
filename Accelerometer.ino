
void getAccelAngles(float *array) {
  float ar = sqrt(array[0]*array[0] + array[1]*array[1] + array[2]*array[2]);
  
  theta[0] = 90.0 - (acos(array[0]/ar)*(180.0/PI));
  theta[1] = 90.0 - (acos(array[1]/ar)*(180.0/PI));
  theta[2] = acos(array[2]/ar)*(180.0/PI);
  theta[0] -= thetazero[0];
  theta[1] -= thetazero[1];
}
