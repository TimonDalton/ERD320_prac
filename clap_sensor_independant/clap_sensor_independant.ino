#include <string>
const int touch_ADC_pin = 27;
const int microphone_ADC_pin = 26;
boolean beingTouched;
int micAdcVal = 0;
template <typename T>
Print& operator<<(Print& printer, T value)
{
  printer.print(value);
  return printer;
} 

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  Serial.println("Start of Serial");

  
    pinMode(microphone_ADC_pin, INPUT);
  //  pinMode(26, OUTPUT);

//  avgChain = new AveragingChain(32);
}

void readMicADCto(int * ptr) {
  pinMode(microphone_ADC_pin, INPUT);
  *ptr = analogRead(microphone_ADC_pin);
}
int counter = 0;
void loop() {
  // put your main code here, to run repeatedly:
  readMicADCto(&micAdcVal);
  if (micAdcVal > 200){//60 for clap only
    Serial << "micADC: "<< micAdcVal <<"\n";
  }
  counter ++;
  if (counter == 50000){
    Serial << "-----------------------avg-"<<"\n";
    counter = 0;
    }
  
}