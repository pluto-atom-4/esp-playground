#include <Arduino.h>
#include <LovyanGFX.hpp>
#include "LGFX_ESP32_RoundDisplay.hpp"

static LGFX lcd;
uint32_t count;

void setup()
{
  lcd.init();
}

void loop()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.printf("%d\r\n", count++);
  delay(1000);
}
