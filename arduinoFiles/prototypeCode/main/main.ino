//Final Prototype
#include <PololuWheelEncoders.h>
#include <NewPing.h>
#include <math.h>

#define INCRE 1.0995574
#define MAX_RANGE 200
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

void movingAverage(float inputValue, float *previousMovingAverage, const float forgettingFactor){
    float currentMovingAverage = (*previousMovingAverage)*forgettingFactor + inputValue; //Calculate Current Moving Average
    *previousMovingAverage = currentMovingAverage; //Sets value of Previous Moving Average pointer to Current Moving Average
}

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
  while(encoders_get_counts_m1() < startRotate1+5 && encoders_get_counts_m2() > startRotate2-5){
    digitalWrite(12, HIGH); //Establishes forward direction of Channel A
    digitalWrite(9, LOW);   //Disengage the Brake for Channel A
    analogWrite(3, 75);   
    
    digitalWrite(13, LOW);  //Establishes backward direction of Channel B
    digitalWrite(8, LOW);   //Disengage the Brake for Channel B
    analogWrite(11,75);   
  }
  digitalWrite(9, HIGH);   //Disengage the Brake for Channel A
  
  digitalWrite(8, HIGH);   //Disengage the Brake for Channel B
}

void moveTo(){
  int initSteps = encoders_get_counts_m2();
  //int distance = (sonar[0].ping_cm());
  int distance = 10000;
  int numSteps = (int)(distance/INCRE);
  int previousE1, previousE2;
  int currentE1, currentE2;
  Serial.println(numSteps);
  int finalSteps = initSteps+numSteps;
  Serial.println(finalSteps);
  digitalWrite(12, HIGH); //running
  digitalWrite(9, LOW);  
  analogWrite(3, 75);   
  digitalWrite(13, HIGH); 
  digitalWrite(8, LOW);   
  analogWrite(11, 75);
  float currentDeltaE1 = 0;
  float currentDeltaE2 = 0;
  previousE1 = encoders_get_counts_m1();
  previousE2 = encoders_get_counts_m2();
  while(encoders_get_counts_m2() < finalSteps){
    currentE1 = encoders_get_counts_m1();
    currentE2 = encoders_get_counts_m2();
    movingAverage((float)(-previousE1+currentE1), &currentDeltaE1, 0);
    movingAverage((float)(-previousE2+currentE2), &currentDeltaE2, 0);
    previousE1 = currentE1;
    previousE2 = currentE2;
    analogWrite(3, 75);    
    analogWrite(11, (currentDeltaE1/currentDeltaE2 > 254 ? 255.0/90.0 : pow(currentDeltaE1/currentDeltaE2,5)) * 90);
    Serial.print(encoders_get_counts_m1());
    Serial.println(encoders_get_counts_m2());   
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
int x = 0;
void loop() {
  Serial.println(sonar[0].ping_cm());
//  while(!x){
//    digitalWrite(12, HIGH); //Establishes forward direction of Channel A
//    digitalWrite(9, LOW);   //Disengage the Brake for Channel A
//    analogWrite(3, 75);   
//    
//    digitalWrite(13, LOW);  //Establishes backward direction of Channel B
//    digitalWrite(8, LOW);   //Disengage the Brake for Channel B
//    analogWrite(11,75); 
//    x = sonar[0].ping_cm();
//    if(x!=0){
//      digitalWrite(9, HIGH);
//      digitalWrite(8, HIGH);
//    }
//  }
//  digitalWrite(12, HIGH); //running
//  digitalWrite(9, LOW);  
//  analogWrite(3, 75);   
//  digitalWrite(13, HIGH); 
//  digitalWrite(8, LOW);   
//  analogWrite(11, 75);
//  delay(10); 
//  while(!objectInFront(0)){
//    rotate(encoders_get_counts_m1(),encoders_get_counts_m2());
//    delay(100);
//  }
  delay(100);
  moveTo();
  delay(10);
}
