void idleState(){


// SNC 16 (0|1|0) 
// DAT1 = 1 if touch
// detected, else DAT1 = 0.
// DAT1 = 0: Remain IDLE
// DAT0 = Designed
// operating speed vop
// when DAT1 = 1

  // auto cb = createControlByte(0,1,0);
  // dp.controlByte.val = 16;
  // dp.dat1 = 0;//no touch detected
  // dp.dat0 = 0;
  // dp.dec = 0;
  // Serial << dp;
  
  bool touched = false;
  while(!touched){
    touched = getTouched();
  }  
  
  // receive();

  dp.controlByte.val = 16;
  dp.dat1 = 1;//touch detected
  dp.dat0 = 1;
  dp.dec = 1;
  Serial << dp;
  delay(20);
  receive();

  currentState = calibration;
}

