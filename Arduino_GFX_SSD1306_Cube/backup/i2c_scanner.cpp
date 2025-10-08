#include <Arduino.h>
#include <Wire.h>

#define I2C_SDA 6 // GPIO pin for SDA
#define I2C_SCL 7 // GPIO pin for SCL

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("I2C Scanner");
  
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);
}

void loop() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000); // wait 5 seconds for next scan
}