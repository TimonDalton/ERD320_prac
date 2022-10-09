
  //start of maze
void mazeState(){
  bool go_to_start = false;
  while(!go_to_start){  
    //SNC 145 (2|1|1)
  /*
    DAT1 = 1 if Clap/Snap was detected, else DAT1 = 0.
    DAT1 = 1: System moves to SOS state
    DAT1 = 0: SNC moves to Touch Detection
        
    DAT1 contains whether a clap or snap was detected.

  */

    dp.controlByte.val = 145;
    checkForClap();
    if (clapped){
      dp.dat1 = 1;
      dp.dat0 = 0;
      dp.dec = 0;
    }else{
      dp.dat1 = 0;
      dp.dat0 = 0;
      dp.dec = 0;
    }
    Serial << dp;
    
    if(clapped){    
      currentState = sos;
      return;
      
    }

//  SNC 146(2|1|2)
/*
DAT1 = 1 if touch detected, else DAT1 = 0. 
DAT1 = 1: System and Subsystems move to IDLE state.
DAT1 = 0: SNC moves to navigation control state (NAVCON)

DAT1 contains whether a touch was detected. 
DAT1 = 1: SNC moves to Touch Detection in IDLE state after clearing DAT1.

*/
    dp.controlByte.val = 146;
    
    updateTouch();
    if(beingTouched){
      dp.dat1 = 1;
      dp.dat0 = 0;
      dp.dec = 0;
    }else{
      dp.dat1 = 0;
      dp.dat0 = 0;
      dp.dec = 0;
    }
    Serial << dp;

    if(beingTouched){    
      currentState = idle;
      return;      
    }
// SNC 147(2|1|3) (NAVCON).
/*
DEC = 0 or 1: DATA bytes contain tangential wheel speeds.
0 = forward 1 = backward DEC = 2 or 3: DATA bytes
contain required angle of rotation of the MARV
2 = left (CCW, positive).
3 = right (CW, negative).

if DEC = 0 or 1:
DAT1 = right wheel speed in mm/s,
DAT0 = left wheel speed in mm/s
if DEC = 2 or 3:
DATA = <DAT1:DAT0> contains the angle of
rotation in degrees between 5° and 360°.
*/
    dp.controlByte.val = 147;
    calculateMove();

    dp.dec = currentMove;
    dp.dat1 = currentMoveD1;
    dp.dat0 = currentMoveD0;

    Serial << dp;

  receive();//MDPS->161(2|2|1)
    /*
    DAT1 = 0
    DAT0 = 0
    DEC = 0
    */

  receive();//MDPS->162(2|2|2)
/*
DATA = <DAT1:DAT0> contains the last measured executed
rotation angle.
DEC = 2: Rotation was to the left (CCW, positive).
DEC = 3: Rotation was to the right (CW, negative).

The executed rotation angle refers to the
actual measured angle the MARV rotated
after having received a rotation instruction
from the SNC.
SNC displays the angle on indicators.
*/
  int rotation = 256*dp.dat1;
  rotation += dp.dat0
  if (dp.dec == 2){
    sign *= 1;
  }else{
    sign *= -1;
  }

  updateUsingActualRotatedAngle(rotation);


  receive(); //MDPS->163(2|2|3)
/*
DATA bytes contain
measured tangential
speeds of the wheels.

DAT1 = right wheel
speed in mm/s,
DAT0 = left wheel
speed in mm/s.
SNC displays the
speeds on indicators.
*/
  displaySpeed(dp.dat1,dp.dat0);


  receive(); //MDPS->164(2|2|4)

/*
  MDPS. MARV Measured DISTANCE moved since last stop/rotation.
  DATA = <DAT1:DAT0> contains measured distance in mm.
  It is the distance the MARV travelled in a straight line to the nearest mm.
*/

  int dTravelled = -256*dp.dat1;
  dTravelled += dp.dat0;

  updateUsingDistanceTravelled(dTravelled);

    if(getEndOfMaze()){
      //Sensor->179(2|3|3)
      receive();

      // dp.controlByte.val = 179;
      // dp.dat1 = 3;
      // dp.dat0 = 3;
      // dp.dec = 3;
      // Serial << dp;

      //Hub->1(0|0|1)
      /*
      Subsystems move to end of maze state.
      */
      receive();

      currentState = hubInit;
      return
    }

    receive(); //Sensor 177(2|3|1)
    
    /*
    DATA = <DAT1:DAT0> contains colour sensed by each sensor as a 3 bit code:
    W = White = 000
    R = Red = 001
    G = Green = 010
    B = Blue = 011
    K = Black = 100

    DATA<2:0> = sensor5
    DATA<5:3> = sensor 4
    DATA<8:6> = sensor 3
    DATA<11:9> = sensor 2
    DATA<14:12> = sensor 1    
    */    
    updateColoursRead(dp.dat1,dp.dat0);

    //Sensor 178(2|3|2) Sens. INCIDENCE 
    /*
    DAT1 contains the last measured angle of incidence.
    SNC displays the last measured angle of
    incidence in degrees on indicators.
    */
    receive();
    displayAngleOfIncidence(dp.dat1);
    updateAngleOfIncidence(dp.dat1)l

    currentState = maze;
    return
      
  }  
}