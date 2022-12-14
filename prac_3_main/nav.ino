bool getEndOfMaze(){
  return true;
}



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
void calculateMove(){
  if (true){
    currentMove = forward;
  }
  if(currentMove == forward || currentMove == backward){
    int rightWheelSpeed = 2;
    int leftWheelSpeed = 2;

    currentMoveD0 = leftWheelSpeed;
    currentMoveD1 = rightWheelSpeed;
  }else{
    //Angle of rotation <D1:D0>
    currentMoveD1 = 0;
    currentMoveD0 = 1;
  }
}

void updateUsingActualRotatedAngle(int angle){

}

void updateUsingDistanceTravelled(int dTravelled){

}

void updateColoursRead(u_int D1,u_int D2){
  int bits[17];
  for (int i =0;i<8;i++){
    bits[i] = bitRead(D1,i);
    bits[i+8] = bitRead(D0,i);
  }

  for (int i = 5;i>=0;i--){
    for (int j=0;j<3;j++){
      u_int x = 0;
      bitWrite(x,j,bits[i*3+j]);
      colourArray[i] = constColourArr[x];      
    }
  }
}

void updateAngleOfIncidence(int angle){
  
}

void navigateNavcon(){
//move forward
  if(navConData.lineColour == green || navConData.lineColour == red){
    if(navConData.incidenceAngle >= 5){
      if (navConData.lineColour == red){
        mazeEnd = true;      
        endMaze();
      }else{
        //forward        
      }
    }else{
      if(navConData.incidenceAngle <= 45){
        stopInMaze();
        reverseInMaze();
        correct5deg();
        //FW
      }else{
        stopInMaze();
       reverseInMaze();
        correct15deg();
        //FW        
      }
    }
  }else if (navConData.lineColour == blue || navConData.lineColour == black){
    if (navConData.incidenceAngle <= 45){
      stopInMaze();
      reverseInMaze();
      correct90plusTheta();
    }else{
      stopInMaze();
      reverseInMaze();
      correct90minTheta();      
    }
  }else{
    //move forward
    
  }
}

void endMaze(){}

void stopInMaze(){}

void reverseInMaze(){}

void correct5deg(){}

void correct15deg(){}

void correct90minTheta(){}

void correct90plusTheta(){}
