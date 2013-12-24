//// VARIABLES
//
//double axpeak[3];
//double axmax;
//double axmin;
//
//boolean axmaxchanged = false;
//boolean axminchanged = false;
//
//double faxpeak[3];
//double faxmax;
//double faxmin;
//
//boolean faxmaxchanged = false;
//boolean faxminchanged = false;  
//
//double aypeak[3];
//double aymax;
//double aymin;
//
//boolean aymaxchanged = false;
//boolean ayminchanged = false;
//
//double faypeak[3];
//double faymax;
//double faymin;
//
//boolean faymaxchanged = false;
//boolean fayminchanged = false;  
//
//double azpeak[3];
//double azmax;
//double azmin;
//
//boolean azmaxchanged = false;
//boolean azminchanged = false;
//
//double fazpeak[3];
//double fazmax;
//double fazmin;
//
//boolean fazmaxchanged = false;
//boolean fazminchanged = false;  
//
//void lowPassFilter(double accelX, double accelY, double accelZ){
//  
//  if(axpeak[0] != accelX) {
//    for(int x=2; x>0; x--) {
//      axpeak[x] = axpeak[x-1];
//    }
//    axpeak[0] = accelX;
//  }
//  
//  if(axpeak[1] < axpeak[2] && axpeak[1] < axpeak[0]){
//    axmin = axpeak[1];
//    axminchanged = true;
//  }
//  
//  if(axpeak[1] > axpeak[2] && axpeak[1] > axpeak[0]) {
//    axmax = axpeak[1];
//    axmaxchanged = true;
//  }
//  
//  if(axminchanged && axmaxchanged){
//    axminchanged = false;
//    axmaxchanged = false;
//    fax = (axmin + axmax) / 2;
//  }
//  
//  if(aypeak[0] != accelY) {
//    for(int x=2; x>0; x--) {
//      aypeak[x] = aypeak[x-1];
//    }
//    aypeak[0] = accelY;
//  }
//  
//  if(aypeak[1] < aypeak[2] && aypeak[1] < aypeak[0]){
//    aymin = aypeak[1];
//    ayminchanged = true;
//  }
//  
//  if(aypeak[1] > aypeak[2] && aypeak[1] > aypeak[0]) {
//    aymax = aypeak[1];
//    aymaxchanged = true;
//  }
//  
//  if(ayminchanged && aymaxchanged){
//    ayminchanged = false;
//    aymaxchanged = false;
//    fay = (aymin + aymax) / 2;
//  }
//  
//  if(azpeak[0] != accelZ) {
//    for(int x=2; x>0; x--) {
//      azpeak[x] = azpeak[x-1];
//    }
//    azpeak[0] = accelZ;
//  }
//  
//  if(azpeak[1] < azpeak[2] && azpeak[1] < azpeak[0]){
//    azmin = azpeak[1];
//    azminchanged = true;
//  }
//  
//  if(azpeak[1] > azpeak[2] && azpeak[1] > azpeak[0]) {
//    azmax = azpeak[1];
//    azmaxchanged = true;
//  }
//  
//  if(azminchanged && azmaxchanged){
//    azminchanged = false;
//    azmaxchanged = false;
//    faz = (azmin + azmax) / 2;
//  }
//}
//
//
//void lowPassFilter2(double faccelX, double faccelY, double faccelZ){
//  
//  if(faxpeak[0] != faccelX) {
//    for(int x=2; x>0; x--) {
//      faxpeak[x] = faxpeak[x-1];
//    }
//    faxpeak[0] = faccelX;
//  }
//  
//  if(faxpeak[1] < faxpeak[2] && faxpeak[1] < faxpeak[0]){
//    faxmin = faxpeak[1];
//    faxminchanged = true;
//  }
//  
//  if(faxpeak[1] > faxpeak[2] && faxpeak[1] > faxpeak[0]) {
//    faxmax = faxpeak[1];
//    faxmaxchanged = true;
//  }
//  
//  if(faxminchanged && faxmaxchanged){
//    faxminchanged = false;
//    faxmaxchanged = false;
//    ffax = (faxmin + faxmax) / 2;
//  }
//  
//  if(faypeak[0] != faccelY) {
//    for(int x=2; x>0; x--) {
//      faypeak[x] = faypeak[x-1];
//    }
//    faypeak[0] = faccelY;
//  }
//  
//  if(faypeak[1] < faypeak[2] && faypeak[1] < faypeak[0]){
//    faymin = faypeak[1];
//    fayminchanged = true;
//  }
//  
//  if(faypeak[1] > faypeak[2] && faypeak[1] > faypeak[0]) {
//    faymax = faypeak[1];
//    faymaxchanged = true;
//  }
//  
//  if(fayminchanged && faymaxchanged){
//    fayminchanged = false;
//    faymaxchanged = false;
//    ffay = (faymin + faymax) / 2;
//  }
//  
//  if(fazpeak[0] != faccelZ) {
//    for(int x=2; x>0; x--) {
//      fazpeak[x] = fazpeak[x-1];
//    }
//    fazpeak[0] = faccelZ;
//  }
//  
//  if(fazpeak[1] < fazpeak[2] && fazpeak[1] < fazpeak[0]){
//    fazmin = fazpeak[1];
//    fazminchanged = true;
//  }
//  
//  if(fazpeak[1] > fazpeak[2] && fazpeak[1] > fazpeak[0]) {
//    fazmax = fazpeak[1];
//    fazmaxchanged = true;
//  }
//  
//  if(fazminchanged && fazmaxchanged){
//    fazminchanged = false;
//    fazmaxchanged = false;
//    ffaz = (fazmin + fazmax) / 2;
//  }
//}
