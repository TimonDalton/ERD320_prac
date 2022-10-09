void displayCurrentState(){
  u8x8.clear();
  u8x8.setCursor(5,0); //horisontal, vertical
  u8x8.print("STARTUP");
}

void displaySpeed(int left,int right){
  u8x8.clear();
  u8x8.setCursor(5,0); //horisontal, vertical
  u8x8.print("SPEED L AND R");
}

void displayAngleOfIncidence(int angle){
  u8x8.clear();
  u8x8.setCursor(5,0); //horisontal, vertical
  u8x8.print("angle incided");
}