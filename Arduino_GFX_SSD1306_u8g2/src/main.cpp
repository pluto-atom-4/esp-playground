#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerifBoldItalic18pt7b.h>  // Example custom font

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// I2C address for most SSD1306 OLEDs is 0x3C
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);

  // Wire begin with correct pins for XIAO ESP32C3
  Wire.begin(6, 7); // SDA = 6, SCL = 7

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    while (1);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // Use custom font
  display.setFont(&FreeSerifBoldItalic18pt7b);
  display.setCursor(0, 40); // y is baseline
  display.println("Hello!");
  display.display();
}

void loop() {
  // nothing here
}