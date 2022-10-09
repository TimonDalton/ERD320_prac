#include <string>
const int touch_ADC_pin = 27;
const int microphone_ADC_pin = 26;
boolean waiting;
int micAdcVal;

template<typename T>
Print& operator << (Print& printer,T value){
  printer.print(value);
  return printer;  
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  Serial.println("Start of Serial"); 
   waiting = false;
    pinMode(microphone_ADC_pin, INPUT);
}

int previouslyCheckedTime = 0;
int timeSinceLastCall() {
  int t = micros();
  int ret = t - previouslyCheckedTime;
  previouslyCheckedTime = t;
  return ret;
}

void readMicADCto(int * ptr) {
  pinMode(microphone_ADC_pin, INPUT);
  *ptr = analogRead(microphone_ADC_pin);
}
int counter = 0;
const int limit = 242;
void loop() {
  readMicADCto(&micAdcVal);
 if (micAdcVal > limit){//60 for clap only
   Serial << "micADC: "<< micAdcVal <<"\n";
 }
  counter ++;
  if (counter == 50000){   
    Serial << "micADC right now: "<< micAdcVal <<"\n";
    counter = 0;
    }

  if (micros() - previouslyCheckedTime < 800000){
    waiting = true;
    }else{
    waiting = false;    
  }
  if (micAdcVal > limit  && !waiting) { //typically goes from 6000 to 50 on touch
      Serial.println("Clap Sensed");
      waiting = true;
      timeSinceLastCall();
  }
}
