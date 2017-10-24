#include "PololuWheelEncoders.h"
#include <NewPing.h>

#define MAX_RANGE 200
#define SENSOR1_TRIG 3
#define SENSOR1_ECHO 4
#define SENSOR2_TRIG 7
#define SENSOR2_ECHO 6
#define LED_GREEN 13
#define LED_RED 12
#define LED_BLUE 11

NewPing sonar[2]={
NewPing(SENSOR1_TRIG, SENSOR1_ECHO, MAX_RANGE),
NewPing(SENSOR2_TRIG, SENSOR2_ECHO, MAX_RANGE)
};

void setLed(int green, int red, int blue){
  digitalWrite(LED_GREEN, green);
  digitalWrite(LED_RED, red);
  digitalWrite(LED_BLUE, blue);
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
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
}
 
void loop() {
  printSensorData();
  
  if(sonar[0].ping_cm() == 0){  
    setLed(HIGH,LOW,LOW);
  }
  else if(sonar[0].ping_cm() < 50){
    setLed(LOW,HIGH,LOW);
  }
  else{
    setLed(LOW,LOW,HIGH);
  }
  delay(33);
}
