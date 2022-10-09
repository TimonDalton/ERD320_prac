void idleState(){
  receive();
  while(receivedPacket.controlByte.val != 0){}

  auto cb = createControlByte(0,1,0);
  DataPacket dp;
  dp.controlByte.val = 16;
  dp.dat1 = 0;//no touch detected
  dp.dat0 = 0;
  dp.dec = 0;
  Serial << dp;
  
  bool touched = false;
  while(!touched){
    touched = getTouched();
  }  
  
  dp.controlByte.val = 16;
  dp.dat1 = 1;//touch detected
  dp.dat0 = 0;
  dp.dec = 0;
  Serial << dp;

  currentState = calibration;
}