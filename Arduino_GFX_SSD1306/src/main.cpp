#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display width and height, change if yours differs
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// I2C address for most SSD1306 displays
#define OLED_ADDR   0x3C

// For XIAO ESP32C3, SDA = 6, SCL = 7
#define OLED_SDA    6
#define OLED_SCL    7

// Create the display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // Set I2C pins before any I2C operation
  Wire.begin(OLED_SDA, OLED_SCL);

  Serial.begin(115200);
  delay(200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Hello, XIAO!"));
  display.display();
}

void loop() {
  // Simple animation example
  static int x = 0;
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("Adafruit GFX"));
  display.fillRect(x, 40, 20, 20, SSD1306_WHITE);
  display.display();
  x = (x + 2) % (SCREEN_WIDTH - 20);
  delay(100);
}