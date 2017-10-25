//Final Prototype
#include <PololuWheelEncoders.h>
#include <NewPing.h>

#define INCRE 1.0995574
#define MAX_RANGE 50
#define SENSOR1_TRIG 7
#define SENSOR1_ECHO 10
//#define SENSOR2_TRIG 7
//#define SENSOR2_ECHO 6
#define ENCODER_1A 2
#define ENCODER_1B 4
#define ENCODER_2A 5
#define ENCODER_2B 6

NewPing sonar[2]={
NewPing(SENSOR1_TRIG, SENSOR1_ECHO, MAX_RANGE),
NewPing(SENSOR1_TRIG, SENSOR1_ECHO, MAX_RANGE)
};

int objectInFront(int sensor){
  int count = 0;
  for(int i = 0;i < 5; i++){
    if(sonar[sensor].ping_cm()!=0){
      count++;
    }
  }
  if(count >= 2){
    return 1;
  }
  else{
    return 0;
  }
}

void printSensorData(){
  Serial.print("Ping1: ");
  Serial.print(sonar[0].ping_cm());
  Serial.print(" ");
  delay(33);
  Serial.print("cm and ");
  Serial.print("Ping2: ");
  Serial.print(sonar[1].ping_cm());
  Serial.print(" cm");
  Serial.println();
  delay(33);
}


void rotate(int startRotate1, int startRotate2){
  while(encoders_get_counts_m1() < startRotate1+10 && encoders_get_counts_m2() > startRotate2-10){
    digitalWrite(12, HIGH); //Establishes forward direction of Channel A
    digitalWrite(9, LOW);   //Disengage the Brake for Channel A
    analogWrite(3, 123);   
    
    digitalWrite(13, LOW);  //Establishes backward direction of Channel B
    digitalWrite(8, LOW);   //Disengage the Brake for Channel B
    analogWrite(11, 123);   
  }
  digitalWrite(9, HIGH);   //Disengage the Brake for Channel A
  
  digitalWrite(8, HIGH);   //Disengage the Brake for Channel B
}

void moveTo(){
  int initSteps = encoders_get_counts_m2();
  int distance = (sonar[0].ping_cm());
  int numSteps = (int)(distance/INCRE);
  Serial.println(numSteps);
  int finalSteps = initSteps+numSteps;
  Serial.println(finalSteps);
  while(encoders_get_counts_m2() < finalSteps){
    Serial.println(encoders_get_counts_m2());
    digitalWrite(12, HIGH); //running
    digitalWrite(9, LOW);  
    analogWrite(3, 255);   
    digitalWrite(13, HIGH); 
    digitalWrite(8, LOW);   
    analogWrite(11, 255);   
  }
  digitalWrite(9,HIGH);//braking
  digitalWrite(8,HIGH);
}

void setup() {
  Serial.begin(9600);
  //Setup Channel A
  pinMode(12, OUTPUT); // Motor Channel A pin
  pinMode(9, OUTPUT); // Brake Channel A pin
  //Setup Channel B
  pinMode(13, OUTPUT); // Motor Channel A pin
  pinMode(8, OUTPUT);  // Brake Channel A pin
  encoders_init(ENCODER_1A, ENCODER_1B, ENCODER_2A, ENCODER_2B);
}
 
void loop() {
  while(!objectInFront(0)){
    rotate(encoders_get_counts_m1(),encoders_get_counts_m2());
    delay(100);
  }
  Serial.println("Object Found");
  moveTo();
  delay(10000000);
}
