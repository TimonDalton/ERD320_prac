void hubInitState(){
  Serial2.print("hehe");
// HUB 0 (0|0|0)
/*
System and Subsystems
move to IDLE state.
*/
  receive();
  while(dp.controlByte.val != 0){}


  currentState = idle;
}