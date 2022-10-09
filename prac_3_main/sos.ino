void sosState(){    
    //do SOS and start while loop over

    receive();//MDPS 228 (3|2|4)

    bool clapLoop = false;
    while (!clapLoop){

      //SNC 208 (3|1|0)SNC. Clap/Snap Detection (SOS).
      /*
      DAT1 = 1 if Clap/Snap,
      else DAT1 = 0.
      DAT1 = 1: System and
      Subsystems move to
      MAZE state.      

      DAT1 = 1: Clap/Snap
      was detected.
      SNC moves to MAZE
      Clap/Snap detection
      after clearing DAT1.
      DAT1 = 0: Wait/loop
      */
      
      clapped = getClapped();
      clapLoop = clapped;

      dp.controlByte.val = 208;
      if(clapped){
        dp.dat1 = 1;
        dp.dat0 = 0;
        dp.dec = 0;
      }else{
        dp.dat1 = 0;
        dp.dat0 = 0;
        dp.dec = 0;
      }
      Serial << dp;

    }
    currentState = maze;
}