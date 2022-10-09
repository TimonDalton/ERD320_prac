void calibrationState(){
  //start of cal
  receive();//expects 112(1|3|0) from sensor
  receive();//expects 96(1|2|0) from MDPS
  /*
    MDPS. vop. (CAL) vop is the default forward tangential wheel speed.  
    DATA bytes contain measured tangential speeds of the wheels.
    Control packet sent after calibration is complete.
    DAT1 = right wheel vR in mm/s, DAT0 = left wheel vL in mm/s.
  */

  bool touched = false;
  
  while(!touched){
    receive();//MDPS->97(1|2|1); Data = 0,0,0
    receive();//Sensor->113(1|3|1)
/*
DATA = <DAT1:DAT0>
contains colour sensed
by each sensor as a 3 bit
code:
W = White = 000
R = Red = 001
G = Green = 010
B = Blue = 011
K = Black = 100
DATA<2:0> = sensor 5
DATA<5:3> = sensor 4
DATA<8:6> = sensor 3
DATA<11:9> = sensor 2
DATA<14:12>=sensor 1
*/        

// SNC 80 (1|1|0)
/*
DAT1 = 1 if touch detected, else DAT1 = 0. DAT1 = 0: Remain IDLE
DAT0 = Designed operating speed vop when DAT1 = 1
*/
    dp.controlByte.val = 80;
    dp.dat1 = 1;
    dp.dat0 = 1;
    dp.dec = 0;
    Serial << dp;
    
    touched = getTouched();
  }

  currentState = maze;

}