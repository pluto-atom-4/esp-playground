#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(10);
}

void loop(){
  Serial.println("Hello World");
  delay(1000);
}
