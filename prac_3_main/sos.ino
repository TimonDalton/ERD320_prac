void sosState(){    
    //do SOS and start while loop over

    dp.controlByte.val = 228;
    dp.dat1 = 3;
    dp.dat0 = 3;
    dp.dec = 3;
    Serial << dp;
  
    dp.controlByte.val = 208;
    dp.dat1 = 3;
    dp.dat0 = 3;
    dp.dec = 3;
    Serial << dp;

    while (!getClap()){}
    currentState = idle;
}