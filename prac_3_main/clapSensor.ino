bool getClap(){//implement commented out version later
  // if(clapped){return true;}else{
  //   while(micros() - generalTimer < 4000){
  //     checkForClap();
  //   }
  //   return clapped;
  // }
  delay(1000);
  return false; 
}

int timeSinceLastClapCall() {
  int t = micros();
  int ret = t - previouslyCheckedClapTime;
  previouslyCheckedClapTime = t;
  return ret;
}

void readMicADCto(int * ptr) {
  pinMode(microphone_ADC_pin, INPUT);
  *ptr = analogRead(microphone_ADC_pin);
}
const int limit = 242;
void checkForClap() {
  readMicADCto(&micAdcVal);

  if (micros() - previouslyCheckedClapTime > 500000){
    clapped = false;    
  }
  if (micAdcVal > limit  && !clapped) {
      clapped = true;
      timeSinceLastClapCall();
  }
}
