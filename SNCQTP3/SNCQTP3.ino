#include "hardware/adc.h"
#include <U8x8lib.h>
#include <Wire.h>
#include <SPI.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <arduino.h>
#define PI 3.1415926535897932384626433832795

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);


//VARIABLE DECLARE START
byte data[4];        //datapack consisting of 4 bytes [data[0],data[1],data[2],data[4]] = [CONTROL, DATA1, DATA0, DEC]
char sensor[5]= {'W','W','W','W','W'};
char currentCol = 'W';
int incidence = 0;
char side = 'R';
bool finishLine = false;
int count = 0;

//for QTP2&4
int sensorNum;
int sensorNum_prev=6;
int currentCol_prev='H';
int incidence_prev = 700;
bool case45 = false;
bool case45_RG = false;
bool case45_BK = false;

//for QTP3
int incidenceStore;
bool BK_L45;
bool BK_L45_2;
bool BK_TA;
bool turnAttempt;



//for TouchSensor
const int touch_ADC_pin = 26;
bool beingTouched;
bool touchDetected = false;
int previouslyCheckedTime = 0;
int ADC_capSenseVal;
int counter = 0;

//ClapSensor
int adcPin = 28;
float conversion = (5.0 / 1023.0)/1.5151;
int sampleAmount = 8;
bool clapDetected = false;
float previous[10] = {0,0,0,0,0,0,0,0,0,0};

bool checkCLAP = false;

//Movement
bool gotoMAZE = false;
bool gotoIDLE = false;
bool gotoCAL = false;
bool gotoSOS = false;

//others
int distanceTravelled;
int rotation;
int wheelspeed1;
int wheelspeed2;
int finalValue = 0;

//HELPER FUNCTIONS DECLARE START
void send(int CONTvalue,int DATA1value,int DATA0value,int DECvalue){
    u8x8.clearLine(6);
    u8x8.setCursor(0,6); //horisontal, vertical
    u8x8.print("TX");
    u8x8.setCursor(3,6); //horisontal, vertical
    u8x8.print(CONTvalue);
    u8x8.setCursor(6,6); //horisontal, vertical
    u8x8.print(DATA1value);
    u8x8.setCursor(9,6); //horisontal, vertical
    u8x8.print(DATA0value);
    u8x8.setCursor(12,6); //horisontal, vertical
    u8x8.print(DECvalue);
    
    
    Serial.flush();
    data[0] = byte(CONTvalue);       //CONT value
    data[1] = byte(DATA1value);      //DATA1 value
    data[2] = byte(DATA0value);      //DATA0 value
    data[3] = byte(DECvalue);        //dec value
    Serial.write(data, 4);  //Write all 4 values
    Serial.flush();       // clear all serial comms so no interference (*/Safety Measure*/)
    return;
}

bool checkClapSnap(){
  checkCLAP = true;
  delay(3000);
  checkCLAP = false;
  if(clapDetected == true){
    u8x8.setCursor(11,3); //horisontal, vertical
    u8x8.print("C");
  }
  else{
    if(touchDetected == true){
    u8x8.setCursor(7,3); //horisontal, vertical
    u8x8.print("/");
    }
  }
  clapDetected = false;
  return clapDetected;
  
}

bool checkTouch(){
  if(touchDetected == true){
    u8x8.setCursor(10,3); //horisontal, vertical
    u8x8.print("T");
  }else{
    if(touchDetected == true){
    u8x8.setCursor(6,3); //horisontal, vertical
    u8x8.print("/");
    
    }
  }
  return touchDetected;
}
//Set the value of each colour
void setSensors(byte DATA1, byte DATA0){
  int byte1[8];
  int byte0[8];
  
  
  //Read values into arrays
  for(int i = 0;i<8;i++){
    byte1[i] = bitRead(int(DATA1), i);
    byte0[i] = bitRead(int(DATA0), i); 
   }
   
  //0,1,2   3,4,5   6,7,8   9,10,11  12,13,14   NOTE THIS IS FLIPPED
  //0,1,2   3,4,5   6,7,0   1,2,3    4,5,6      NOTE THIS IS FLIPPED
  //<DATA1,DATA0>

  sensor[4] = getColour(byte0[2],byte0[1],byte0[0]);
  sensor[3] = getColour(byte0[5],byte0[4],byte0[3]);
  sensor[2] = getColour(byte1[0],byte0[7],byte0[6]);
  sensor[1] = getColour(byte1[3],byte1[2],byte1[1]);
  sensor[0] = getColour(byte1[6],byte1[5],byte1[4]);

  setCurrentColourAndSide();
  

 return;
}

char getColour(int b2, int b1, int b0){
  if(b2 == 0){
    if(b1 == 0){
      if(b0 == 0){
        return 'W'; //000
      }else{
        return 'R'; //001
      }
    }else{
      if(b0 == 0){
        return 'G'; //010
      }else{
        return 'B'; //011
      }
    }
  }else{
    return 'K'; //1xx
  }
}

void setCurrentColourAndSide(){
  for(int i = 0;i<5;i++){
    if(sensor[i] != 'W'){
      currentCol = sensor[i];
      if(i<2){
        side = 'L';
      }else{
        side = 'R';
      }
      sensorNum = i;
      return;
    }else{
      currentCol = 'W';
    }
  }
}

int byteToInt(int D1, int D2){

  uint8_t bits1 = 8;
  uint8_t bits0 = 8;
  uint8_t i = 0;
  uint8_t k = 0;
  char xDAT[17];
  char xDAT1[9];
  char xDAT0[9];
  xDAT1[0] = 0;
  xDAT0[0] = 0;
  xDAT[0] = 0;

  while ( bits1-- )
    xDAT1[ i++ ] = bitRead( D1, bits1 ) + '0';
  xDAT1[ i ] = '\0';

  while ( bits0-- )
    xDAT0[ k++ ] = bitRead( D0, bits0 ) + '0';
  xDAT0[ k ] = '\0';

  //itoa(D1,xDAT1,2);
  //itoa(D0,xDAT0,2);


  strcpy(xDAT, xDAT1);
  strcat(xDAT, xDAT0);


 finalValue = strtol(xDAT, NULL, 2);

  return finalValue;
  
}


void wacko_mode(){
  //THEN I GUESS UR SHIT FUCKED FUCK YOU
    case45 = true;
    
    
    if(currentCol == 'R' || currentCol == 'G' || case45_RG == true){
      u8x8.clearLine(0);
      u8x8.setCursor(0,4); //horisontal, vertical
      u8x8.print("MAZE_navi_wRG");
      u8x8.setCursor(14,4); //horisontal, vertical
      u8x8.print(count);
        case45_RG = true;
        if(count == 0){
        send(147,0,0,0);//STOP
        count = count+1;
        }else if(count == 1){
          send(147,100,100,1);//REVERSE
          count = count+1;
        }else if(count == 2){
          send(147,0,0,0);//STOP
          count = count+1;
        }else if(count == 3){
          if(side == 'L'){
            send(147,0,5,2);//LEFT 5
            count = count+1;
          }else if(side == 'R'){
            send(147,0,5,3);//RIGHT 5
            count = count+1;
          }
        }else if(count == 4){
          send(147,100,100,0);//FORWARD
          count = 0;
          case45 = false;
          case45_RG = false;
        }
      

    }else if(currentCol == 'K' || currentCol == 'B' || case45_BK == true){
      u8x8.clearLine(0);
      u8x8.setCursor(0,4); //horisontal, vertical
      u8x8.print("MAZE_navi_wBK");
      u8x8.setCursor(14,4); //horisontal, vertical
      u8x8.print(count);
        case45_BK = true;
        
        if(count == 0){
          send(147,100,100,0);//FORWARD
          count = count+1;
        }else if(count == 1){
          send(147,0,0,0);//STOP
          count = count+1;
        }else if(count == 2){
          send(147,100,100,1);//REVERSE
          count = count+1;
        }else if(count == 3){
          send(147,0,0,0);//STOP
          count = count+1;
        }else if(count == 4){
          if(side == 'L'){
            send(147,0,5,3);//RIGHT 5
            count = count+1;
          }else if(side == 'R'){
            send(147,0,5,2);//LEFT 5
            count = count+1;
          }
        }else if(count == 5){
          send(147,100,100,0);//FORWARD
          count = 0;
          case45 = false;
          case45_BK = false;
        }
    }

    currentCol_prev = currentCol;
    incidence_prev = incidence;
    sensorNum_prev = sensorNum;
    return;
}

void navigate(){
  u8x8.clearLine(0);
  u8x8.setCursor(0,0); //horisontal, vertical
  u8x8.print("MAZE_navi");

  //DEC =0 backward.
  //DEC =1 forward.
  //DAT1 = rightWheel speed mm/s.
  //DAT0 = leftWheel speed mm/s.

  //DEC =2 left (CCW, positive).
  //DEC =3 right (CW, negative).
  //<DATA1:DAT0> angle of rotation in degrees between 5 & 360


  if((BK_L45 == true && currentCol == 'B') || (BK_L45 == true && currentCol == 'K')){
    BK_L45_2 = true;
  }else{
    BK_L45 = false;
  }


  if(BK_L45_2 == true ){
    u8x8.setCursor(0,0); //horisontal, vertical
    u8x8.print("MAZE_navi_BK_45");
    
    if(count == 0){
//      requiredDistance = distanceTravelled + (sin(incidence*(PI/180))*20)
//      if((requiredDistance > 20)
      send(147,100,100,1); // REVERSE
      count = count+1;
    }else if(count == 1){
      send(147,0,0,0); // STOP
      count = count+1;
    }else if(count == 2){
      if(side == 'L'){
        send(147,0,(90-incidenceStore),3);//RIGHT x
        count = 0;
        turnAttempt = true;
        BK_L45 = false;
        BK_L45_2 = false;
      }else if(side == 'R'){
        send(147,0,(90+incidenceStore),3);//RIGHT x
        count = 0;
        turnAttempt = true;
        BK_L45 = false;
        BK_L45_2 = false;
      }
      
    }
    return;
  }


  if(sensorNum == sensorNum_prev && currentCol == currentCol_prev && incidence == incidence_prev && incidence == 0 && currentCol != 'W'){
      case45 = true;
  }
  
  if(case45 == true){
      wacko_mode();
      return;
  }



  if(BK_TA == true){
    u8x8.setCursor(0,0); //horisontal, vertical
    u8x8.print("MAZE_navi_BK_TA");
    if(count == 0){
      send(147,100,100,1); // REVERSE
      count = count +1;
    }else if(count == 1){
      send(147,0,180,3); //180 RIGHT
      count = count +1;
    }else if(count == 2){
      send(147,100,100,0); // FORWARD
      count =0;
      BK_TA = false;
    }
    return;
  }
  
    
  
u8x8.clearLine(4);

  if(currentCol == 'B'||currentCol == 'K'){
      u8x8.clearLine(0);
      u8x8.setCursor(0,0); //horisontal, vertical
      u8x8.print("MAZE_navi_BK");
    
      if(turnAttempt == true){
        u8x8.print("MAZE_navi_BK_att");
        
          turnAttempt = false;
          BK_TA = true;
          send(147,0,0,0);//
      }else if(incidence == 0){
          u8x8.print("MAZE_navi_BK_i=0");
          send(147,0,0,0);
      
      }else if(incidence < 45){
          u8x8.print("MAZE_navi_BK_L45");
          send(147,0,0,0); // STOP
          BK_L45 = true;
          incidenceStore = incidence;
        
      }

  }else if(currentCol == 'R'||currentCol == 'G'){
      u8x8.clearLine(0);
      u8x8.setCursor(0,0); //horisontal, vertical
      u8x8.print("MAZE_navi_RG");
//      u8x8.setCursor(13,0); //horisontal, vertical
//      u8x8.print(incidence);

    
      turnAttempt = false;
    
      if(45>incidence && incidence>5){
        if(count ==0){
           send(147,0,0,0);//STOP
           count = count+1;
        }else if(count == 1){
          send(147,100,100,1);//REVERSE
          count = count+1;
        }else if(count == 2){
          send(147,0,0,0);//STOP
          count = count+1;
        }else if(count == 3){
          if(side == 'L'){
            send(147,0,incidence,2);//LEFT 
          }else if(side == 'R')
            send(147,0,incidence,3);//RIGHT 
          count = 0;
        }
        
      }else if(incidence<=5){
          if(currentCol == 'R'){
              finishLine = true;
              ;
          }
          
          send(147,100,100,0);
          //sendMessage('FORWARD',True)
      }
   }else if(currentCol == 'W'){
      u8x8.clearLine(0);
      u8x8.setCursor(0,0); //horisontal, vertical
      u8x8.print("MAZE_navi_W");
      u8x8.setCursor(11,0); //horisontal, vertical
      u8x8.print(count);
     
      if(finishLine == true){
         u8x8.setCursor(0,0); //horisontal, vertical
         u8x8.print("MAZE_navi_EOM");
        
          if(count == 0 ){
            send(147,0,0,0); //STOP
            count = count+1;
          }
          else if(count == 1 ){
            send(147,1,104,3); //360 RIGHT
            count = 0;
          }
      }else{
        u8x8.setCursor(13,0); //horisontal, vertical
       
        send(147,100,100,0); // FORWARD
      }
   }

   currentCol_prev = currentCol;
   incidence_prev = incidence;
   sensorNum_prev = sensorNum;
   return;
} 

//STATE FUNCTIONS DECLARE START
bool IDLE_state(){
  u8x8.setCursor(0,0); //horisontal, vertical
  u8x8.print("IDLE");
  if(checkTouch() == true){
        touchDetected = false;
        send(16,1,0,0);
        return true;
  }else{
        send(16,0,0,0);
        return false;
  }
}

bool CAL_state(){
  u8x8.setCursor(0,0); //horisontal, vertical
  u8x8.print("CAL");
  if(checkTouch() == true){
        touchDetected = false;
        send(80,1,0,0);//ENTER MAZE
        return true;
  }else{
        send(80,0,0,0);//STAY IN CAL
        return false;
  }
}

bool MAZE_state(){
  u8x8.setCursor(0,0); //horisontal, vertical
  u8x8.print("MAZE");
  if(checkClapSnap() == false){
 
      send(145,0,0,0);// STAY IN MAZE
      if(checkTouch() == false){
      
        send(146,0,0,0); //STAY IN MAZE
        navigate();
        return false;
      }else{
        touchDetected = false;
        send(146,1,0,0); //GOTO IDLE
        return true;
      }
  }else{
    clapDetected = false;
    send(145,1,0,0);//ENTER SOS
    return false;
  }
}

bool SOS_state(){
  u8x8.setCursor(0,0); //horisontal, vertical
  u8x8.print("SOS");
  if(checkClapSnap() == true){
        clapDetected = false;
        send(208,1,0,0);
        return true;
  }else{
        
        send(208,0,0,0);
        return false;
  }
}



//MAIN CORE
void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);//Small Font 16 Characters
  u8x8.noInverse();//Normal
  u8x8.clear();
  u8x8.setCursor(5,0); //horisontal, vertical
  u8x8.print("STARTUP");
  
}

void loop() {
  float value = (analogRead(A1)*conversion)*100;
//  Serial.print(value);
//  delay(300);
//  Serial.print("\n");
       
//SERIAL RECIEVE
  if(Serial.available() == 4){
      data[0] = byte(Serial.read());   //control byte
      data[1] = byte(Serial.read());
      data[2] = byte(Serial.read());
      data[3] = byte(Serial.read());

      u8x8.clearLine(5);
      u8x8.setCursor(0,5); //horisontal, vertical
      u8x8.print("RX");
      u8x8.setCursor(3,5); //horisontal, vertical
      u8x8.print(int(data[0]));
      u8x8.setCursor(6,5); //horisontal, vertical
      u8x8.print(int(data[1]));
      u8x8.setCursor(9,5); //horisontal, vertical
      u8x8.print(int(data[2]));
      u8x8.setCursor(12,5); //horisontal, vertical
      u8x8.print(int(data[3]));


      //delay(500);
      

    //IDLE STATE
    if(data[0] == byte(0)){
      u8x8.clearLine(0);
      u8x8.setCursor(0,0); //horisontal, vertical
      u8x8.print("IDLE");
      
      gotoCAL = IDLE_state();
      while(gotoCAL == false){
        delay(200); //delay for 0.2s
        gotoCAL = IDLE_state();
      }
      u8x8.clearLine(0);
      u8x8.setCursor(0,0); //horisontal, vertical
      u8x8.print("LEFT IDLE");
    }


    //CAL STATE
    if(data[0] == byte(113)){

      
      u8x8.clearLine(0);
      u8x8.setCursor(0,0); //horisontal, vertical
      u8x8.print("CAL");
      setSensors(data[1],data[2]);
      u8x8.setCursor(0,3); //horisontal, vertical
      u8x8.print(sensor[0]);
      u8x8.setCursor(1,3); //horisontal, vertical
      u8x8.print(sensor[1]);
      u8x8.setCursor(2,3); //horisontal, vertical
      u8x8.print(sensor[2]);
      u8x8.setCursor(3,3); //horisontal, vertical
      u8x8.print(sensor[3]);
      u8x8.setCursor(4,3); //horisontal, vertical
      u8x8.print(sensor[4]);
    
      u8x8.setCursor(6,3); //horisontal, vertical
      u8x8.print(currentCol);
      u8x8.setCursor(7,3); //horisontal, vertical
      u8x8.print(side);

      
      gotoMAZE = CAL_state();
      
      if (gotoMAZE == true){
        
        sensor[0] = 'W';
        sensor[1] = 'W';
        sensor[2] = 'W';
        sensor[3] = 'W';
        sensor[4] = 'W';
        currentCol = 'W';
        side = 'R';
        u8x8.clearLine(0);
        u8x8.setCursor(0,0); //horisontal, vertical
        u8x8.print("MAZE");
        delay(500);
        
        //MAZE STATE
        gotoIDLE = MAZE_state();
        
        if(gotoIDLE == true){
          u8x8.clearLine(0);
          u8x8.setCursor(0,0); //horisontal, vertical
          u8x8.print("IDLE");
          //delay(500);
          
          
          gotoCAL = IDLE_state();
          while(gotoCAL == false){
            
            delay(200); //delay for 0.5s
            gotoCAL = IDLE_state();
          }
          
        }
      }
    }


   //MAZE STATE
   if(data[0] == byte(178)){
      incidence = int(data[1]);
      u8x8.setCursor(9,3); //horisontal, vertical
      u8x8.print(incidence);
      u8x8.clear();
      
        u8x8.setCursor(0,0); //horisontal, vertical
        
      
      u8x8.print("MAZE");
      delay(500);
      gotoIDLE = MAZE_state();
        
      if(gotoIDLE == true){
        u8x8.clearLine(0);
        u8x8.setCursor(0,0); //horisontal, vertical
        u8x8.print("IDLE");
        delay(500);
        
        gotoCAL = IDLE_state();
        while(gotoCAL == false){
          delay(200); //delay for 0.5s
          gotoCAL = IDLE_state();
        }
      }
   }
   //SOS STATE
   if(data[0] == byte(228)){
      u8x8.clear();
      u8x8.setCursor(0,0); //horisontal, vertical
      u8x8.print("SOS");
      gotoMAZE = SOS_state();
      while(gotoMAZE == false){
        delay(200); //delay for 0.5s
        gotoMAZE = SOS_state();
      }
      
      //MAZE STATE
      gotoIDLE = MAZE_state();
      
      if(gotoIDLE == true){
        u8x8.clear();
        u8x8.setCursor(0,0); //horisontal, vertical
        u8x8.print("IDLE");

        gotoCAL = IDLE_state();
        while(gotoCAL == false){
          delay(200); //delay for 0.5s
          gotoCAL = IDLE_state();
        }
      }
   }


//OTHERS


//colours

  if(data[0] == byte(177)){
    setSensors(data[1],data[2]);
    u8x8.setCursor(0,3); //horisontal, vertical
    u8x8.print(sensor[0]);
    u8x8.setCursor(1,3); //horisontal, vertical
    u8x8.print(sensor[1]);
    u8x8.setCursor(2,3); //horisontal, vertical
    u8x8.print(sensor[2]);
    u8x8.setCursor(3,3); //horisontal, vertical
    u8x8.print(sensor[3]);
    u8x8.setCursor(4,3); //horisontal, vertical
    u8x8.print(sensor[4]);

    u8x8.setCursor(6,3); //horisontal, vertical
    u8x8.print(currentCol);
    u8x8.setCursor(7,3); //horisontal, vertical
    u8x8.print(side);
  }

  //rotation
  if(data[0] == byte(162)){
    rotation = byteToInt(int(data[1]),int(data[2]) );
    u8x8.setCursor(0,1); //horisontal, vertical
    u8x8.print(rotation);
  }
  
  //speed
  if(data[0] == byte(163)){
    wheelspeed1 = int(data[1]);
    wheelspeed2 = int(data[2]);
    
    u8x8.setCursor(0,2); //horisontal, vertical
    u8x8.print(wheelspeed1);
    u8x8.setCursor(4,2); //horisontal, vertical
    u8x8.print(wheelspeed2);//ends on 6
  }

  //distance
  if(data[0] == byte(164)){
    distanceTravelled = byteToInt(int(data[1]),int(data[2]) );
    u8x8.setCursor(8,2); //horisontal, vertical
    u8x8.print(distanceTravelled);
  }

    
  }//end serial available check
}//end loop

//INterrupt
void setup1() {
  pinMode(touch_ADC_pin, OUTPUT);
  beingTouched = false;     
  getStarted();
}

void loop1() {
  if(checkCLAP == true){
      clapSnapFunction();
  }else{
      touchFunction();
  }
  
}




//CLAP SNAP
void clapSnapFunction(){
  float value = (analogRead(A1)*conversion)*100;
  
  int count =0;

  for(int i =0;i<10;i++){
    
    if(value>(previous[i]+7)){
      count = count+1;
    }    
  }

  if(count == 10){
      clapDetected = true;
      delay(2000);
//      Serial.print(value);
//      Serial.print("mV          CLAP DETECTED");
//      Serial.print("\n");
      
  }
  
  previous[9] = previous[8];
  previous[8] = previous[7];
  previous[7] = previous[6];
  previous[6] = previous[5];
  previous[5] = previous[4];
  previous[4] = previous[3];
  previous[3] = previous[2];
  previous[2] = previous[1];
  previous[1] = previous[0];
  previous[0] = value;
  
}

void getStarted(){
  for(int i =0;i<20;i++){
     float value = (analogRead(A1)*conversion)*100;
      previous[9] = previous[8];
      previous[8] = previous[7];
      previous[7] = previous[6];
      previous[6] = previous[5];
      previous[5] = previous[4];
      previous[4] = previous[3];
      previous[3] = previous[2];
      previous[2] = previous[1];
      previous[1] = previous[0];
      previous[0] = value;
  }
 return;
}


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
  if(vale>1010){
    touchDetected = true;
    delay(2000);
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
