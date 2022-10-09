bool getTouched(){
  // updateTouch();
  // return beingTouched;
  return true;
  }
bool getTouch(){
  return getTouched();
  }

// ////pin setup
// #include <string>


int timeSinceLastCallT() {
  int t = micros();
  int ret = t - previouslyCheckedTimeT;
  previouslyCheckedTimeT = t;
  return ret;
}

int ADC_capSenseVal;

void charge() {
  pinMode(touch_ADC_pin, OUTPUT);
  digitalWrite(touch_ADC_pin, HIGH);
  delay(5);
//    Serial << "+CHARGED+ to " << analogRead(touch_ADC_pin) <<"\n";
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
int counter = 0;

void updateTouch(){
  charge();
  readTouchADCto(&ADC_capSenseVal);
  previouslyCheckedTimeT = micros();
  while (ADC_capSenseVal > 200) {
    dischargeSlightly();
    readTouchADCto(&ADC_capSenseVal);
  }
  int t = timeSinceLastCallT();
//    Serial.println(t);
  if (t < 250 && !beingTouched) { //typically goes from 6000 to 50 on touch
    Serial.println("SENSED TOUCH");
    beingTouched = true;
  }else{
      if (t < 250 )
        beingTouched = true;
      else
        beingTouched = false;
  }
}