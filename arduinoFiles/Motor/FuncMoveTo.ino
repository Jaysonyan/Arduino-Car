#define INCRE 1.0995574

void MoveTo(){
  int initSteps = encoders_get_counts_m1();
  int distance = sonar[0].ping_cm();
  int numSteps = (int)(distance/INCRE);
  int finalSteps = initSteps+numSteps;
  while(encoder_get_counts_m1() != numsteps){
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

