
//TOUCH

int timeSinceLastCall() {
  int t = micros();
  int ret = t - previouslyCheckedTime;
  previouslyCheckedTime = t;
  return ret;
}


void charge() {
  pinMode(touch_ADC_pin, OUTPUT);
  digitalWrite(touch_ADC_pin, HIGH);
  delay(5);

 float vale = analogRead(touch_ADC_pin);
  if(vale>1000){
    touchDetected = true;
    delay(1000);
  }
  
}
void dischargeSlightly() {
  pinMode(touch_ADC_pin, OUTPUT);
  digitalWrite(touch_ADC_pin, LOW);
  delayMicroseconds(40);
}
void readTouchADCto(int * ptr) {
  pinMode(touch_ADC_pin, INPUT);
  *ptr = analogRead(touch_ADC_pin);
}
//int counter = 0;

void touchFunction(){//updateTouchFunction
  charge();
  readTouchADCto(&ADC_capSenseVal);
  previouslyCheckedTime = micros();
  while (ADC_capSenseVal > 200) {
    dischargeSlightly();
    readTouchADCto(&ADC_capSenseVal);
  }
  int t = timeSinceLastCall();
}
