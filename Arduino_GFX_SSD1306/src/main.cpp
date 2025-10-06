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

#define OLED_RESET -1

// Create the display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void drawPolygon(int16_t cx, int16_t cy, int16_t radius, int sides) {
  if (sides < 3) return;  // need at least 3 sides

  float angleStep = 2 * PI / sides;
  int16_t x0 = cx + radius * cos(0);
  int16_t y0 = cy + radius * sin(0);

  for (int i = 1; i <= sides; i++) {
    float angle = i * angleStep;
    int16_t x1 = cx + radius * cos(angle);
    int16_t y1 = cy + radius * sin(angle);
    display.drawLine(x0, y0, x1, y1, SSD1306_WHITE);
    x0 = x1;
    y0 = y1;
  }
}

void setup() {
  Serial.begin(115200);

  delay(200);

  // Set I2C pins before any I2C operation
  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }

  display.clearDisplay();
  // Draw title
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Polygon Demo"));

  // Draw polygons
  drawPolygon(64, 32, 20, 3); // triangle
  drawPolygon(64, 32, 30, 6); // hexagon overlay

  display.display();
}

void loop() {
  // nothing here
}