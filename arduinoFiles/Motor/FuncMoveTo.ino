
void MoveTo(){
  int initSteps = encoder.getCountsAndResetM1();
  int distance = sonar[0].ping_cm();
  int numSteps = distance/INCRE;
  while(numsteps != encoder.getCounts()){
    digitalWrite(12, HIGH); //Establishes forward direction of Channel A
    digitalWrite(9, LOW);   //Disengage the Brake for Channel A
    analogWrite(3, 255);   //Spins the motor on Channel A at full speed
    digitalWrite(13, HIGH); //Establishes forward direction of Channel B
    digitalWrite(8, LOW);   //Disengage the Brake for Channel B
    analogWrite(11, 255);   //Spins the motor on Channel B at full speed
  }
  digitalWrite(9,HIGH);
  digitalWrite(8,HIGH);
}

