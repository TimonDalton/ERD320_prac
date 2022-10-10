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
bool mazeEnd;
int micAdcVal;

enum ColourCodes{
  white = 0,
  red = 1,
  green = 2,
  blue = 3,
  black = 4//k
};

struct NavCon_Data{
    int distanceTravelled;
    int angleRotated;
    int incidenceAngle;
    bool hasTurnedAroundOnce;
    ColourCodes lineColour;
        

    
};
NavCon_Data navConData;

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
  printer.flush();
  printer.write(value.controlByte.val);
  printer.write(value.dat1);
  printer.write(value.dat0);
  printer.write(value.dec);
  printer.flush();
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

DataPacket dp;

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
        dp.controlByte.val = byte(serial_in.toInt());
        dp.dat1 = byte(serial_in.toInt());
        dp.dat1 = byte(serial_in.toInt());
        dp.dec = byte(serial_in.toInt());
      }
    }
}

enum Moves {//possible moves to make in navcon
  forward,
  backward,
  left,
  right
};
Moves currentMove = forward;
int currentMoveD0 = 0;
int currentMoveD1 = 0;

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
  hubInit,
  idle,
  calibration,
  maze,
  sos
};


const char constColourArr[]  = {'W','R','G','B','K'};

char colourArray[5];
int currentState = idle;

void loop() {

  if(micros() - generalTimer > 5000){
    generalTimer = micros();
    checkForClap();
    updateTouch();
  }

  if (currentState == hubInit){
    hubInitState();    
  }else
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
    sosState();
  }
}
