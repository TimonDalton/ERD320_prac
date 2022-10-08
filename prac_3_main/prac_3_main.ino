#include <string>
const int touch_ADC_pin = 26;
const int microphone_ADC_pin = 27;
boolean waiting;
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

int * in_packet = new int[4];
int * out_packet = new int[4];

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

void setup() {
  Serial.begin(19200);

  // Serial.println("Start of Serial");
}

void loop() {
start_of_loop:
  receive();
  auto cb = createControlByte(0,1,0);

  DataPacket dp;
  dp.controlByte.val = 16;
  dp.dat1 = 0;
  dp.dat0 = 0;
  dp.dec = 0;
  Serial << dp;
  
  bool touched = false;
  while(!touched){
    touched = getTouched();
  }  
  
  dp.controlByte.val = 16;
  dp.dat1 = 1;
  dp.dat0 = 0;
  dp.dec = 0;
  Serial << dp;

//start of cal
  receive();
    
  
  dp.controlByte.val = 112;
  dp.dat1 = 0;
  dp.dat0 = 0;
  dp.dec = 0;
  Serial << dp;
  receive();
    
  dp.controlByte.val = 96;
  dp.dat1 = 3;
  dp.dat0 = 3;
  dp.dec = 3;

  touched = false;
  
  while(!touched){
    dp.controlByte.val = 97;
    dp.dat1 = 3;
    dp.dat0 = 3;
    dp.dec = 3;
    Serial << dp;
    
    dp.controlByte.val = 113;
    dp.dat1 = 3;
    dp.dat0 = 3;
    dp.dec = 3;
    Serial << dp;
    
    dp.controlByte.val = 80;
    dp.dat1 = 3;
    dp.dat0 = 3;
    dp.dec = 3;
    Serial << dp;
    
    touched = getTouched;
  }

  
  //start of maze
  
  bool go_to_start = false;
  while(!go_to_start){  
    dp.controlByte.val = 145;
    dp.dat1 = 3;
    dp.dat0 = 3;
    dp.dec = 3;
    Serial << dp;
    
    if(getClap()){
      //do SOS and start while loop over
  
      dp.controlByte.val = 228;
      dp.dat1 = 3;
      dp.dat0 = 3;
      dp.dec = 3;
      Serial << dp;
    
      dp.controlByte.val = 208;
      dp.dat1 = 3;
      dp.dat0 = 3;
      dp.dec = 3;
      Serial << dp;
  
      while (!getClap()){}
    
      continue;
      
    }
    
      dp.controlByte.val = 147;
      dp.dat1 = 3;
      dp.dat0 = 3;
      dp.dec = 3;
      Serial << dp;

      dp.controlByte.val = 161;
      dp.dat1 = 3;
      dp.dat0 = 3;
      dp.dec = 3;
      Serial << dp;

      dp.controlByte.val = 162;
      dp.dat1 = 3;
      dp.dat0 = 3;
      dp.dec = 3;
      Serial << dp;

      dp.controlByte.val = 163;
      dp.dat1 = 3;
      dp.dat0 = 3;
      dp.dec = 3;
      Serial << dp;

      dp.controlByte.val = 164;
      dp.dat1 = 3;
      dp.dat0 = 3;
      dp.dec = 3;
      Serial << dp;

      if(getEndOfMaze()){
        dp.controlByte.val = 179;
        dp.dat1 = 3;
        dp.dat0 = 3;
        dp.dec = 3;
        Serial << dp;
        

        dp.controlByte.val = 1;
        dp.dat1 = 3;
        dp.dat0 = 3;
        dp.dec = 3;
        Serial << dp;

        goto start_of_loop;
      }

      dp.controlByte.val = 177;
      dp.dat1 = 3;
      dp.dat0 = 3;
      dp.dec = 3;
      Serial << dp;

      dp.controlByte.val = 178;
      dp.dat1 = 3;
      dp.dat0 = 3;
      dp.dec = 3;
      Serial << dp;

      dp.controlByte.val = 145;
      dp.dat1 = 3;
      dp.dat0 = 3;
      dp.dec = 3;
      Serial << dp;

      
  }  


}
