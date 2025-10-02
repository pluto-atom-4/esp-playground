#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include "NotoSansMonoSCB16pt7b.h" 

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define I2C_SDA 4
#define I2C_SCL 5

// Use I2C SSD1306
Arduino_DataBus *bus = new Arduino_ESP32I2C(I2C_SDA, I2C_SCL);
Arduino_GFX *gfx = new Arduino_SSD1306_128X64_I2C(bus, -1, 0x3C);

void setup() {
  Serial.begin(115200);
  gfx->begin();
  gfx->fillScreen(BLACK);
  gfx->setTextColor(WHITE);
  gfx->setFont(&NotoSansMonoSCB16pt7b); // Set Japanese font

  // Example Japanese string in UTF-8
  gfx->setCursor(0, 24);
  gfx->println("こんにちは世界"); // "Hello, World" in Japanese

  // If your font doesn't support all glyphs, you may see '?' marks.
}

void loop() {
  // Nothing to do here
}