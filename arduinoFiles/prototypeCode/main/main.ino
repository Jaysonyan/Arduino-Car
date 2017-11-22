#include <PololuWheelEncoders.h>
#include <NewPing.h>
#include <math.h>

// Declaring all constants used in the code
#define INCRE 3
#define MAX_RANGE 50
#define SENSOR1_TRIG 7
#define SENSOR1_ECHO 10
#define ENCODER_1A 2
#define ENCODER_1B 4
#define ENCODER_2A 5
#define ENCODER_2B 6
#define LED_GREEN 17
#define LED_RED 18
#define LED_BLUE 16
#define SPEED_ADJUST_MULTIPLIER 5
#define MEASURED_ENCODER_FULL_TURN 108.0

//Sonar sensor object
NewPing sonar = NewPing(SENSOR1_TRIG, SENSOR1_ECHO, MAX_RANGE);

//Normalizes array of distances to hold preference to larger objects
void takeMovingAverageInArray(int a[], int n){
  int *t = (int*)malloc(n*sizeof(int));
  for(int i = 0; i < n; i++){
    t[i] = (a[i] + a[(i+1)%n] + a[(i-1+n)%n])/3;
  }
  for(int i = 0; i < n; i++){
    a[i] = t[i];
  }
  free(t);
  t = NULL;
}

//Find closest distnace that is not zero
int findIndexOfLowestNonZero(int a[], int n){
  int minIndex = -1;
  for(int i = 0; i < n; i++){
    if(a[i] != 0){
      if(minIndex == -1 || a[i] < a[minIndex]){
        minIndex = i;
      }
    }
  }
  return minIndex;
}

//Rotates around 360 degrees in search of an object
/*angle in degrees
speed from 0 to 255
direction true - clockwise, false - counterclockwise*/
int* rotate(float angle, int speed, bool direction, bool scan){
  int startE1 = encoders_get_counts_m1();
  int startE2 = encoders_get_counts_m2();
  int currentE1, currentE2;

  int *distance = (int *)malloc((int)(angle/360.0 * MEASURED_ENCODER_FULL_TURN) * sizeof(int));
  for(int i = 0; i < ((int)(angle/360.0 * MEASURED_ENCODER_FULL_TURN)); i++){
    distance[i] = 0;
  }

  digitalWrite(12, direction ? LOW : HIGH);
  digitalWrite(13,  direction ? HIGH : LOW);
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  
  digitalWrite(LED_RED, 0);
  digitalWrite(LED_GREEN, 0);
  digitalWrite(LED_BLUE, 0);
  
  while(encoders_get_counts_m1()*(direction ? -1 : 1) < startE1*(direction ? -1 : 1) + (angle/360 * MEASURED_ENCODER_FULL_TURN) 
  || encoders_get_counts_m2()*(direction ? -1 : 1) > startE2*(direction ? -1 : 1) - (angle/360 * MEASURED_ENCODER_FULL_TURN)){
    
    currentE1 = encoders_get_counts_m1();
    currentE2 = encoders_get_counts_m2();

    analogWrite(3, min(speed + SPEED_ADJUST_MULTIPLIER*(abs(currentE2 - startE2) - abs(currentE1 - startE1)), 255));   
    analogWrite(11,min(speed + SPEED_ADJUST_MULTIPLIER*(abs(currentE1 - startE1) - abs(currentE2 - startE2)), 255));

    if(sonar.ping_cm() != 0){
      digitalWrite(LED_GREEN, 128);
      digitalWrite(LED_RED, 0);
    }
    else{
      digitalWrite(LED_RED, 128);
      digitalWrite(LED_GREEN, 0);
    }
    
    int currentEncoderChangeMin = min(abs(encoders_get_counts_m1() - startE1),abs(encoders_get_counts_m2() - startE2));
    distance[currentEncoderChangeMin] = sonar.ping_cm();
  }
  digitalWrite(9, HIGH);
  digitalWrite(8, HIGH);

  digitalWrite(LED_RED, 0);
  digitalWrite(LED_GREEN, 0);
  digitalWrite(LED_BLUE, 0);
  
  if(!scan){
    free(distance);
    distance = NULL;
  }
  else{
    return distance;
  }
}

//Takes in the distance and moves car that distance
void moveTo(int speed, int distance){
  int numSteps = (int)(distance*INCRE);
  
  int startE1 = encoders_get_counts_m1();
  int startE2 = encoders_get_counts_m2();
  int currentE1, currentE2;

  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH); 
  digitalWrite(9, LOW);  
  digitalWrite(8, LOW);   
  
  digitalWrite(LED_RED, 0);
  digitalWrite(LED_GREEN, 0);
  digitalWrite(LED_BLUE, 0);
  
  while(((currentE1 - startE1) + (currentE2 - startE2))/2 < numSteps){
    currentE1 = encoders_get_counts_m1();
    currentE2 = encoders_get_counts_m2();
    
    analogWrite(3, min(speed + SPEED_ADJUST_MULTIPLIER*(abs(currentE2 - startE2) - abs(currentE1 - startE1)), 255));   
    analogWrite(11,min(speed + SPEED_ADJUST_MULTIPLIER*(abs(currentE1 - startE1) - abs(currentE2 - startE2)), 255));

    int sensorDistance = sonar.ping_cm();
    if(sensorDistance != 0){
      digitalWrite(LED_GREEN, 128);
      digitalWrite(LED_RED, 0);
    }
    else{
      digitalWrite(LED_RED, 128);
      digitalWrite(LED_GREEN, 0);
    }
  
  }
  
  digitalWrite(9,HIGH);
  digitalWrite(8,HIGH);

  digitalWrite(LED_RED, 0);
  digitalWrite(LED_GREEN, 0);
  digitalWrite(LED_BLUE, 0);
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
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
}

void loop() {
  digitalWrite(LED_RED, 0);
  digitalWrite(LED_GREEN, 0);
  digitalWrite(LED_BLUE, 0);
  int *distance;
  //Rotates car around until it finds an object
  do{
    free(distance);
    distance = NULL;
    distance = rotate(360, 60, true, true);
    delay(1000);
  } while(findIndexOfLowestNonZero(distance, MEASURED_ENCODER_FULL_TURN) == -1);

  //Calibrates distance array to make zero-readings unfavourable to closest distance algorithm
  for(int i = 0; i < (int)(360.0/360.0 * MEASURED_ENCODER_FULL_TURN); i++){
    if(distance[i] == 0){
      distance[i] = MAX_RANGE;
    }
  }

  //Prints distance array before moving average function
  for(int i = 0; i < (int)(MEASURED_ENCODER_FULL_TURN); i++){
    Serial.print(distance[i]);
    Serial.print(",");
  }
  Serial.println();

  takeMovingAverageInArray(distance, MEASURED_ENCODER_FULL_TURN);

  //Prints distance array after moving average function
  for(int i = 0; i < (int)(MEASURED_ENCODER_FULL_TURN); i++){
    Serial.print(distance[i]);
    Serial.print(",");
  }
  Serial.println();

  //finds the index of the closest object in the distance array
  int minIndex = findIndexOfLowestNonZero(distance, MEASURED_ENCODER_FULL_TURN);
  Serial.println(minIndex);
  Serial.println(distance[minIndex]);
  Serial.println(((float)(MEASURED_ENCODER_FULL_TURN - minIndex))/MEASURED_ENCODER_FULL_TURN * 360.0);
  
  //calculates how much the car must turn to be facing the closest object
  float rotationRequired = ((float)(minIndex))/MEASURED_ENCODER_FULL_TURN * 360.0;
  rotate(360.0 - rotationRequired, 60, false, false);
  delay(1000);

  //Determines an accurate distance reading of closest object
  int betterDistance = sonar.ping_cm();
  moveTo(60, max(betterDistance,distance[minIndex]));

  digitalWrite(LED_BLUE, 128);
  
  Serial.print(betterDistance);
  free(distance);
  distance = NULL;
  
  delay(10000);
}
