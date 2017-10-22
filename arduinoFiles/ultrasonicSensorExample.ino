#include <NewPing.h>

NewPing sonar[2]={
NewPing(3, 4, 200),
NewPing(7, 6, 200)
};
 
void setup() {
  Serial.begin(9600);
}
 
void loop() {
  delay(33);
  Serial.print("Ping1: ");
  Serial.print(sonar[0].ping_cm());
  Serial.print("cm and ");
  delay(33);
  Serial.print("Ping2: ");
  Serial.print(sonar[1].ping_cm());
  Serial.println("cm");
}
