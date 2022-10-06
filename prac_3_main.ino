#include <string>
const int touch_ADC_pin = 26;
const int microphone_ADC_pin = 27;
boolean waiting;
int micAdcVal;

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
        in_packet[i] = byte(serial_in.toInt());
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
  dp.controlbyte.val = 16;
  dp.dat1 = 3;
  dp.dat0 = 3;
  dp.dec = 3;
  Serial << dp;
  
  bool touched = false;
  while(!touched){
    touched = getTouched();
  }

//start of cal
  
  dp.controlbyte.val = 112;
  dp.dat1 = 3;
  dp.dat0 = 3;
  dp.dec = 3;
  Serial << dp;
    
  dp.controlbyte.val = 96;
  dp.dat1 = 3;
  dp.dat0 = 3;
  dp.dec = 3;

  touched = false;
  
  while(!touched){
    dp.controlbyte.val = 97;
    dp.dat1 = 3;
    dp.dat0 = 3;
    dp.dec = 3;
    Serial << dp;
    
    dp.controlbyte.val = 113;
    dp.dat1 = 3;
    dp.dat0 = 3;
    dp.dec = 3;
    Serial << dp;
    
    dp.controlbyte.val = 80;
    dp.dat1 = 3;
    dp.dat0 = 3;
    dp.dec = 3;
    Serial << dp;
    
    touched = getTouched;
  }

  
  //start of maze
  
  bool go_to_start = false;
  while(!go_to_start){  
    dp.controlbyte.val = 145;
    dp.dat1 = 3;
    dp.dat0 = 3;
    dp.dec = 3;
    Serial << dp;
    
    if(getClap()){
      //do SOS and start while loop over
  
      dp.controlbyte.val = 228;
      dp.dat1 = 3;
      dp.dat0 = 3;
      dp.dec = 3;
      Serial << dp;
    
      dp.controlbyte.val = 208;
      dp.dat1 = 3;
      dp.dat0 = 3;
      dp.dec = 3;
      Serial << dp;
  
      while (!getClap()){}
    
      continue;
      
    }
    
      dp.controlbyte.val = 147;
      dp.dat1 = 3;
      dp.dat0 = 3;
      dp.dec = 3;
      Serial << dp;

      dp.controlbyte.val = 161;
      dp.dat1 = 3;
      dp.dat0 = 3;
      dp.dec = 3;
      Serial << dp;

      dp.controlbyte.val = 162;
      dp.dat1 = 3;
      dp.dat0 = 3;
      dp.dec = 3;
      Serial << dp;

      dp.controlbyte.val = 163;
      dp.dat1 = 3;
      dp.dat0 = 3;
      dp.dec = 3;
      Serial << dp;

      dp.controlbyte.val = 164;
      dp.dat1 = 3;
      dp.dat0 = 3;
      dp.dec = 3;
      Serial << dp;

      if(getEndOfMaze()){
        dp.controlbyte.val = 179;
        dp.dat1 = 3;
        dp.dat0 = 3;
        dp.dec = 3;
        Serial << dp;
        

        dp.controlbyte.val = 1;
        dp.dat1 = 3;
        dp.dat0 = 3;
        dp.dec = 3;
        Serial << dp;

        goto start_of_loop;
      }

      dp.controlbyte.val = 177;
      dp.dat1 = 3;
      dp.dat0 = 3;
      dp.dec = 3;
      Serial << dp;

      dp.controlbyte.val = 178;
      dp.dat1 = 3;
      dp.dat0 = 3;
      dp.dec = 3;
      Serial << dp;

      dp.controlbyte.val = 145;
      dp.dat1 = 3;
      dp.dat0 = 3;
      dp.dec = 3;
      Serial << dp;

      
  }  


}
