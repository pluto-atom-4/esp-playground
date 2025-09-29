#include <Arduino.h>
#include <LovyanGFX.hpp>
#include "LGFX_ESP32_RoundDisplay.hpp"

static LGFX lcd;
uint32_t count;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting setup...");
  Serial.printf("SCLK:%d MOSI:%d MISO:%d DC:%d CS:%d\n", D8, D10, D9, D3, D1);
  lcd.init();
  Serial.println("LCD initialized.");
}

void loop()
{
 // Serial.print("Loop count: ");
 // Serial.println(count);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.printf("%d\r\n", count++);
  delay(1000);
}