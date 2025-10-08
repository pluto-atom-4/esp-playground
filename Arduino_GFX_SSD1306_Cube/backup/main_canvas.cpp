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
// Try 0x3D if 0x3C doesn't work
Arduino_DataBus *bus = new Arduino_Wire(0x3C, 0x00, 0x40);
Arduino_SSD1306 *display = new Arduino_SSD1306(bus, OLED_RESET, SCREEN_WIDTH, SCREEN_HEIGHT);
Arduino_Canvas_Mono *gfx = new Arduino_Canvas_Mono(SCREEN_WIDTH, SCREEN_HEIGHT, display);

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
  Serial.begin(115200);
  Serial.println("Starting SSD1306 Cube Demo...");
  
  // Initialize I2C with custom pins
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000); // Set I2C frequency to 400kHz
  
  Serial.println("Initializing display...");
  if (!display->begin()) {
    Serial.println("SSD1306 allocation failed");
    for(;;); // Don't proceed, loop forever
  }
  
  Serial.println("Display initialized, setting up canvas...");
  gfx->begin();
  gfx->fillScreen(BLACK);
  gfx->flush();
  
  Serial.println("Setup complete!");
}

void loop() {
  static float ax = 0, ay = 0, az = 0;
  static unsigned long lastDebug = 0;
  
  ax += 0.04; ay += 0.03; az += 0.02;

  // Calculate projected vertices
  int px[8], py[8];
  for (int i = 0; i < 8; i++) {
    Vec3 rv = rotate(cube.vertices[i], ax, ay, az);
    project(rv, px[i], py[i]);
  }

  // Clear the canvas
  gfx->fillScreen(BLACK);
  
  // Draw cube edges
  for (int i = 0; i < 12; i++) {
    int x1 = px[cube.edges[i][0]];
    int y1 = py[cube.edges[i][0]];
    int x2 = px[cube.edges[i][1]];
    int y2 = py[cube.edges[i][1]];
    
    // Ensure coordinates are within screen bounds
    if (x1 >= 0 && x1 < SCREEN_WIDTH && y1 >= 0 && y1 < SCREEN_HEIGHT &&
        x2 >= 0 && x2 < SCREEN_WIDTH && y2 >= 0 && y2 < SCREEN_HEIGHT) {
      gfx->drawLine(x1, y1, x2, y2, WHITE);
    }
  }
  
  // Also draw vertices as dots for debugging
  for (int i = 0; i < 8; i++) {
    if (px[i] >= 0 && px[i] < SCREEN_WIDTH && py[i] >= 0 && py[i] < SCREEN_HEIGHT) {
      gfx->fillCircle(px[i], py[i], 1, WHITE);
    }
  }

  // Update the display
  gfx->flush();
  
  // Debug output every 2 seconds
  if (millis() - lastDebug > 2000) {
    Serial.print("Vertices: ");
    for (int i = 0; i < 8; i++) {
      Serial.print("(");
      Serial.print(px[i]);
      Serial.print(",");
      Serial.print(py[i]);
      Serial.print(") ");
    }
    Serial.println();
    lastDebug = millis();
  }
  
  delay(50); // Reduced delay for smoother animation
}