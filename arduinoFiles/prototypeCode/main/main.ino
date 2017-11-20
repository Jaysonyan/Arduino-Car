#include <PololuWheelEncoders.h>
#include <NewPing.h>
#include <math.h>

//
#define INCRE 3

#define MAX_RANGE 50

#define SENSOR1_TRIG 7
#define SENSOR1_ECHO 10

#define ENCODER_1A 2
#define ENCODER_1B 4
#define ENCODER_2A 5
#define ENCODER_2B 6

#define FORGETTING_FACTOR 0.9
#define SPEED_ADJUST_MULTIPLIER 5
#define MEASURED_ENCODER_FULL_TURN 108.0

NewPing sonar = NewPing(SENSOR1_TRIG, SENSOR1_ECHO, MAX_RANGE);

void movingAverage(float inputValue, float *previousMovingAverage, float *previousWeight, const float forgettingFactor){
    float currentMovingAverage = (*previousMovingAverage*forgettingFactor*(*previousWeight) + inputValue)/(forgettingFactor*(*previousWeight) + 1); //Calculate Current Moving Average
    *previousMovingAverage = currentMovingAverage; //Sets value of Previous Moving Average pointer to Current Moving Average
    *previousWeight = forgettingFactor*(*previousWeight) + 1.0; //Sets value of Previous Weight pointer to Current Weight
}

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
  
  while(encoders_get_counts_m1()*(direction ? -1 : 1) < startE1*(direction ? -1 : 1) + (angle/360 * MEASURED_ENCODER_FULL_TURN) 
  || encoders_get_counts_m2()*(direction ? -1 : 1) > startE2*(direction ? -1 : 1) - (angle/360 * MEASURED_ENCODER_FULL_TURN)){
    
    currentE1 = encoders_get_counts_m1();
    currentE2 = encoders_get_counts_m2();

    analogWrite(3, min(speed + SPEED_ADJUST_MULTIPLIER*(abs(currentE2 - startE2) - abs(currentE1 - startE1)), 255));   
    analogWrite(11,min(speed + SPEED_ADJUST_MULTIPLIER*(abs(currentE1 - startE1) - abs(currentE2 - startE2)), 255));

    int currentEncoderChangeMin = min(abs(encoders_get_counts_m1() - startE1),abs(encoders_get_counts_m2() - startE2));
    distance[currentEncoderChangeMin] = sonar.ping_cm();
  }
  digitalWrite(9, HIGH);
  digitalWrite(8, HIGH);
  if(!scan){
    free(distance);
    distance = NULL;
  }
  else{
    return distance;
  }
}

void moveTo(int speed, int distance){
  int numSteps = (int)(distance*INCRE);
  
  int startE1 = encoders_get_counts_m1();
  int startE2 = encoders_get_counts_m2();
  int currentE1, currentE2;

  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH); 
  digitalWrite(9, LOW);  
  digitalWrite(8, LOW);   
  
  while(((currentE1 - startE1) + (currentE2 - startE2))/2 < numSteps){
    currentE1 = encoders_get_counts_m1();
    currentE2 = encoders_get_counts_m2();
    
    analogWrite(3, min(speed + SPEED_ADJUST_MULTIPLIER*(abs(currentE2 - startE2) - abs(currentE1 - startE1)), 255));   
    analogWrite(11,min(speed + SPEED_ADJUST_MULTIPLIER*(abs(currentE1 - startE1) - abs(currentE2 - startE2)), 255));
  }
  
  digitalWrite(9,HIGH);
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
  int *distance;
  do{
    free(distance);
    distance = NULL;
    distance = rotate(360, 60, true, true);
    delay(1000);
  } while(findIndexOfLowestNonZero(distance, MEASURED_ENCODER_FULL_TURN) == -1);
  
  for(int i = 0; i < (int)(360.0/360.0 * MEASURED_ENCODER_FULL_TURN); i++){
    if(distance[i] == 0){
      distance[i] = MAX_RANGE;
    }
  }
  
  for(int i = 0; i < (int)(360.0/360.0 * MEASURED_ENCODER_FULL_TURN); i++){
    Serial.print(distance[i]);
    Serial.print(",");
  }
  Serial.println();
  takeMovingAverageInArray(distance, MEASURED_ENCODER_FULL_TURN);
  for(int i = 0; i < (int)(360.0/360.0 * MEASURED_ENCODER_FULL_TURN); i++){
    Serial.print(distance[i]);
    Serial.print(",");
  }
  Serial.println();
  int minIndex = findIndexOfLowestNonZero(distance, MEASURED_ENCODER_FULL_TURN);
  Serial.println(minIndex);
  Serial.println(distance[minIndex]);
  Serial.println(((float)(MEASURED_ENCODER_FULL_TURN - minIndex))/MEASURED_ENCODER_FULL_TURN * 360.0);
  float rotationRequired = ((float)(minIndex))/MEASURED_ENCODER_FULL_TURN * 360.0;
  rotate(360.0 - rotationRequired, 60, false, false);
  delay(1000);
  int betterDistance = sonar.ping_cm();
  moveTo(60, max(betterDistance,distance[minIndex]));
  Serial.print(betterDistance);
  free(distance);
  distance = NULL;
  
  delay(10000);
  /*
  moveTo(60, 15);
  delay(10000);
  */
}
