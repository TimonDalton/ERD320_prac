////pin setup
#include <string>
const int touch_ADC_pin = 27;
boolean beingTouched;
template <typename T>
Print& operator<<(Print& printer, T value)
{
  printer.print(value);
  return printer;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(touch_ADC_pin, OUTPUT);
  //  pinMode(26, OUTPUT);

  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  Serial.println("Start of Serial");
 beingTouched = false;
}

int previouslyCheckedTime = 0;
int timeSinceLastCall() {
  int t = micros();
  int ret = t - previouslyCheckedTime;
  previouslyCheckedTime = t;
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
void loop() {
  charge();
  readTouchADCto(&ADC_capSenseVal);
  previouslyCheckedTime = micros();
  while (ADC_capSenseVal > 200) {
    dischargeSlightly();
    readTouchADCto(&ADC_capSenseVal);
    int timestamp =  micros() - previouslyCheckedTime;
    //      if (counter = 5000){
    //        Serial  << timestamp << ": ADC capsenseval:" << ADC_capSenseVal <<"\n" ;
    //        counter = 0;
    //      }
    //      counter ++;
  }
  int t = timeSinceLastCall();
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

  //  delay(20);
  //  ADC_capSenseVal = analogRead(touch_ADC_pin);  // read the input pin
  //  Serial.println(ADC_capSenseVal);          // debug value
  //  Serial.println("_______________");
  //  delay(200);
}