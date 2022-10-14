
//CLAP SNAP
void clapSnapFunction(){
  float value = (analogRead(A0)*conversion)*100;
  
  int count =0;

  for(int i =0;i<10;i++){
    
    if(value>(previous[i]+7)){
      count = count+1;
    }    
  }

  if(count == 10){
      clapDetected = true;
      delay(2000);
//      Serial.print(value);
//      Serial.print("mV          CLAP DETECTED");
//      Serial.print("\n");
      
  }
  
  previous[9] = previous[8];
  previous[8] = previous[7];
  previous[7] = previous[6];
  previous[6] = previous[5];
  previous[5] = previous[4];
  previous[4] = previous[3];
  previous[3] = previous[2];
  previous[2] = previous[1];
  previous[1] = previous[0];
  previous[0] = value;
  
}

void getStarted(){
  for(int i =0;i<20;i++){
     float value = (analogRead(A0)*conversion)*100;
      previous[9] = previous[8];
      previous[8] = previous[7];
      previous[7] = previous[6];
      previous[6] = previous[5];
      previous[5] = previous[4];
      previous[4] = previous[3];
      previous[3] = previous[2];
      previous[2] = previous[1];
      previous[1] = previous[0];
      previous[0] = value;
  }
 return;
}
