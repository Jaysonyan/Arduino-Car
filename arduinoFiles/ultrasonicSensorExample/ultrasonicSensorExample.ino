#include "PololuWheelEncoders.h"
#include <NewPing.h>

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
  Serial.println(objectInFront(0));

  delay(33);
}
