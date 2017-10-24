#include <PololuWheelEncoders.h>

PololuWheelEncoders encoders;

void setup() {
  Serial.begin(9600);
  encoders_init(4, 5, 2, 10);
}

void loop() {
  delay(100);
  Serial.println(encoders_get_counts_m1());
}
