void calibrationState(){
  //start of cal
  receive();//expects 112(1|3|0) from sensor.MDPS moves to speed calibration
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
    while (dp.controlByte.val != 113)
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
    int desOpSpd = 2;

    dp.controlByte.val = 80;
    touched = getTouched();
    if(touched){
      dp.dat1 = 1;
      dp.dat0 = desOpSpd;
      dp.dec = 0;
    }else{
      dp.dat1 = 0;
      dp.dat0 = 0;
      dp.dec = 0;
    }
    Serial << dp;
  delay(20);
    
  }

  currentState = maze;

}