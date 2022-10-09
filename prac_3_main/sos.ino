void sosState(){    
    //do SOS and start while loop over

    //MDPS 228 (3|2|4) ->
    receive();
    dp.controlByte.val = 228;
    dp.dat1 = 3;
    dp.dat0 = 3;
    dp.dec = 3;
    Serial << dp;
  
    bool clapped = getClapped();
    while (!clapped){

      //SNC 208 (3|1|0)SNC. Clap/Snap Detection (SOS).
      dp.controlByte.val = 208;
      dp.dat1 = 3;
      dp.dat0 = 3;
      dp.dec = 3;
      Serial << dp;
    }
    currentState = maze;
}