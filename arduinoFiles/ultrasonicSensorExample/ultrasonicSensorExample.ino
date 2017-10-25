#include "PololuWheelEncoders.h"
#include <NewPing.h>

#define MAX_RANGE 200
#define SENSOR1_TRIG 7
#define SENSOR1_ECHO 10
//#define SENSOR2_TRIG 7
//#define SENSOR2_ECHO 6
#define LED_GREEN 13
#define LED_RED 12
#define LED_BLUE 11
#define ENCODER_1A = 2
#define ENCODER_1B = 4
#define ENCODER_2A = 5
#define ENCODER_2B = 6

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


void rotate(startRotate){
  while(encoders_get_counts_m1()>)
  digitalWrite(12, HIGH); //Establishes forward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, 255);   //Spins the motor on Channel A at full speed

  digitalWrite(13, LOW);  //Establishes backward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, 123);    //Spins the motor on Channel B at half speed

}


void setup() {
  Serial.begin(9600);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  
  //Setup Channel A
  pinMode(12, OUTPUT); // Motor Channel A pin
  pinMode(9, OUTPUT); // Brake Channel A pin
  //Setup Channel B
  pinMode(13, OUTPUT); // Motor Channel A pin
  pinMode(8, OUTPUT);  // Brake Channel A pin
  encoders_init(ENCODER_1A, ENCODER_1B, ENCODER_2A, ENCODER_2B);
}
 
void loop() {
  printSensorData();

  delay(33);
}
