
  //start of maze
  void mazeState(){
  bool go_to_start = false;
  while(!go_to_start){  
    dp.controlByte.val = 145;
    dp.dat1 = 3;
    dp.dat0 = 3;
    dp.dec = 3;
    Serial << dp;
    
    if(getClap()){
    
      currentState = sos;
      return;
      
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

        // goto start_of_loop;
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