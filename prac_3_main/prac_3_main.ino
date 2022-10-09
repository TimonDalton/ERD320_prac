#include <string>

#include <U8x8lib.h>
#include <Wire.h>
#include <SPI.h>
#include <arduino.h>

const int touch_ADC_pin = 27;
const int microphone_ADC_pin = 26;

// const int screen_power_pin = 18;
// const int screen_gnd_pin = 19;
// const int screen_SCL_pin = 20;
// const int screen_SDA_pin = 21;

bool beingTouched;
bool clapped;
int micAdcVal;

struct ControlByte {
  int sys,sub,isr;
  unsigned val;
  unsigned getCb(){
  unsigned ret = isr;
  ret += sub*16;
  // ret += sys*64;  
  val = ret;
  return ret;
  }
};

struct DataPacket {
  ControlByte controlByte;
  unsigned dat1,dat0,dec;  
};
Print& operator<<(Print& printer, DataPacket value)
{
  
  printer.write(value.controlByte.val);
  printer.write(value.dat1);
  printer.write(value.dat0);
  printer.write(value.dec);
  return printer;
}
template <typename T>
Print& operator<<(Print& printer, T value)
{
  printer.write(value);
  return printer;
}
// void readMicADCto(int * ptr) {
//   pinMode(microphone_ADC_pin, INPUT);
//   *ptr = analogRead(microphone_ADC_pin);
// }

const unsigned control_byte = 0;
const unsigned dat1 = 1;
const unsigned dat0 = 2;
const unsigned dec = 3;
DataPacket receivedPacket;

void sendPacket(int arr[]){
  for (int i =0;i<4;i++){
    Serial<<arr[i];
  }
}
void sendPacket(int cb,int d1,int d0,int dc){
  for (int i =0;i<4;i++){
    Serial<<cb<<d1<<d0<<dc;
  }
}
unsigned createControlByte(int sys,int sub,int isr){
  unsigned ret = isr;
  ret += sub*16;
  // ret += sys*64;  
  return ret;
}

void receive()
{
    while (Serial.available() == 0) {;}
    {
      String serial_in;
      for (int i = 0;i<4;i++){

      serial_in = Serial.read();
        receivedPacket.controlByte.val = byte(serial_in.toInt());
        receivedPacket.dat1 = byte(serial_in.toInt());
        receivedPacket.dat1 = byte(serial_in.toInt());
        receivedPacket.dec = byte(serial_in.toInt());
      }
    }
}

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
int previouslyCheckedTimeT = 0;
int previouslyCheckedClapTime = 0;
int generalTimer = 0;
void setup() {
  Serial.begin(19200);

  // Serial.println("Start of Serial");
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);//Small Font 16 Characters
  u8x8.noInverse();//Normal
  u8x8.clear();
  u8x8.setCursor(5,0); //horisontal, vertical
  u8x8.print("STARTUP");

  pinMode(touch_ADC_pin, OUTPUT);
  
  beingTouched = false;
  clapped = false;
  pinMode(microphone_ADC_pin, INPUT);

}

enum states {
  idle,
  calibration,
  maze,
  sos
};
int currentState = idle;

void loop() {

  if(micros() - generalTimer > 5000){
    generalTimer = micros();
    checkForClap();
  }  

        
  if (currentState == idle){
    idleState();
  }else
  if (currentState == calibration){
    calibrationState();
  }else        
  if (currentState == maze){
    mazeState();
  }else        
  if (currentState == sos){
    SOSstate();
  }
}
