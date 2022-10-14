
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
