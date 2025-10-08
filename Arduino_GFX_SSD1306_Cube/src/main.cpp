#include <Arduino.h>
#undef LITTLE_FOOT_PRINT  // Ensure Canvas classes are available
#include <Arduino_GFX_Library.h>
#include <Wire.h>

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // Not used
#define I2C_SDA 6 // GPIO pin for SDA
#define I2C_SCL 7 // GPIO pin for SCL

// Use the I2C bus for the SSD1306  
Arduino_DataBus *bus = new Arduino_Wire(0x3C, 0x00, 0x40);
Arduino_SSD1306 *display = new Arduino_SSD1306(bus, OLED_RESET, SCREEN_WIDTH, SCREEN_HEIGHT);
Arduino_GFX *gfx = new Arduino_Canvas_Mono(SCREEN_WIDTH, SCREEN_HEIGHT, display);

struct Vec3 {
  float x, y, z;
};

struct Cube {
  Vec3 vertices[8];
  uint8_t edges[12][2];
};

// Vertices and edges for a unit cube
Cube cube = {
  {
    {-0.5, -0.5, -0.5},
    {+0.5, -0.5, -0.5},
    {+0.5, +0.5, -0.5},
    {-0.5, +0.5, -0.5},
    {-0.5, -0.5, +0.5},
    {+0.5, -0.5, +0.5},
    {+0.5, +0.5, +0.5},
    {-0.5, +0.5, +0.5}
  },
  {
    {0,1},{1,2},{2,3},{3,0},
    {4,5},{5,6},{6,7},{7,4},
    {0,4},{1,5},{2,6},{3,7}
  }
};

Vec3 rotate(Vec3 v, float ax, float ay, float az) {
  // Rotate around X
  float x1 = v.x, y1 = cos(ax)*v.y - sin(ax)*v.z, z1 = sin(ax)*v.y + cos(ax)*v.z;
  // Rotate around Y
  float x2 = cos(ay)*x1 + sin(ay)*z1, y2 = y1, z2 = -sin(ay)*x1 + cos(ay)*z1;
  // Rotate around Z
  float x3 = cos(az)*x2 - sin(az)*y2, y3 = sin(az)*x2 + cos(az)*y2, z3 = z2;
  return {x3, y3, z3};
}

void project(Vec3 v, int &x, int &y) {
  float scale = 40.0;
  float viewer_z = 2.5;
  x = (int)(SCREEN_WIDTH / 2 + scale * v.x / (viewer_z - v.z));
  y = (int)(SCREEN_HEIGHT / 2 + scale * v.y / (viewer_z - v.z));
}

void setup() {
  // Initialize I2C with custom pins
  Wire.begin(I2C_SDA, I2C_SCL);
  
  display->begin();
  gfx->begin();
  gfx->fillScreen(BLACK);
}

void loop() {
  static float ax = 0, ay = 0, az = 0;
  ax += 0.04; ay += 0.03; az += 0.02;

  int px[8], py[8];
  for (int i = 0; i < 8; i++) {
    Vec3 rv = rotate(cube.vertices[i], ax, ay, az);
    project(rv, px[i], py[i]);
  }

  gfx->fillScreen(BLACK);
  // Draw cube edges
  for (int i = 0; i < 12; i++) {
    gfx->drawLine(px[cube.edges[i][0]], py[cube.edges[i][0]],
                  px[cube.edges[i][1]], py[cube.edges[i][1]], WHITE);
  }

  gfx->flush();
  delay(20);
}