#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "I2Cdev.h"
#include "MPU6050.h"

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // Not used for I2C
#define I2C_SDA 6 // GPIO pin for SDA
#define I2C_SCL 7 // GPIO pin for SCL

// Create display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// MPU6050 object
MPU6050 accelgyro;

// Sensor data variables
int16_t ax, ay, az;
int16_t gx, gy, gz;
float ax_filtered = 0, ay_filtered = 0, az_filtered = 0;
float gx_filtered = 0, gy_filtered = 0, gz_filtered = 0;

// Low-pass filter coefficient (adjust for smoothness)
const float alpha = 0.1;

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

void readMPU6050() {
  // Read raw sensor data
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  // Apply low-pass filter to smooth the data
  ax_filtered = alpha * ax + (1 - alpha) * ax_filtered;
  ay_filtered = alpha * ay + (1 - alpha) * ay_filtered;
  az_filtered = alpha * az + (1 - alpha) * az_filtered;
  
  gx_filtered = alpha * gx + (1 - alpha) * gx_filtered;
  gy_filtered = alpha * gy + (1 - alpha) * gy_filtered;
  gz_filtered = alpha * gz + (1 - alpha) * gz_filtered;
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Starting MPU6050 + SSD1306 Cube Demo...");
  
  // Initialize I2C with custom pins
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);
  Serial.println("I2C initialized");
  
  // Initialize MPU6050
  Serial.println("Initializing MPU6050...");
  accelgyro.initialize();
  
  // Verify MPU6050 connection
  if (accelgyro.testConnection()) {
    Serial.println("MPU6050 connection successful");
  } else {
    Serial.println("MPU6050 connection failed");
    // Continue anyway, display will show error
  }
  
  // Initialize display with I2C address 0x3C
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  Serial.println("Display initialized successfully!");
  
  // Test display with startup message
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("MPU6050 + Cube Demo");
  display.println("Initializing...");
  display.display();
  delay(2000);
  
  // Calibrate MPU6050 (simple offset calibration)
  Serial.println("Calibrating MPU6050... Keep sensor still!");
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Calibrating...");
  display.println("Keep sensor still!");
  display.display();
  
  // Simple calibration - take average of first 100 readings as offset
  long ax_offset = 0, ay_offset = 0, az_offset = 0;
  long gx_offset = 0, gy_offset = 0, gz_offset = 0;
  
  for(int i = 0; i < 100; i++) {
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    ax_offset += ax;
    ay_offset += ay;
    az_offset += az;
    gx_offset += gx;
    gy_offset += gy;
    gz_offset += gz;
    delay(10);
  }
  
  // Store offsets (we'll subtract these from future readings)
  ax_offset /= 100;
  ay_offset /= 100;
  az_offset = (az_offset / 100) - 16384; // Z should read ~16384 when upright
  gx_offset /= 100;
  gy_offset /= 100;
  gz_offset /= 100;
  
  Serial.println("Calibration complete!");
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Calibration complete!");
  display.println("Starting cube...");
  display.display();
  delay(1000);
  
  Serial.println("Setup complete!");
}

void loop() {
  static unsigned long lastDebug = 0;
  static unsigned long lastUpdate = 0;
  
  // Read sensor data at 50Hz
  if (millis() - lastUpdate >= 20) {
    readMPU6050();
    lastUpdate = millis();
  }
  
  // Convert gyroscope data to rotation angles
  // Scale down the gyroscope values for smooth rotation
  float rotation_x = gx_filtered * 0.0001; // Adjust these scaling factors as needed
  float rotation_y = gy_filtered * 0.0001;
  float rotation_z = gz_filtered * 0.0001;
  
  // Alternative: Use accelerometer for tilt-based rotation
  // Uncomment these lines and comment the gyro lines above for tilt control
  // float rotation_x = ay_filtered * 0.0002;
  // float rotation_y = ax_filtered * 0.0002; 
  // float rotation_z = 0; // Keep Z rotation from gyro or set to 0

  // Calculate projected vertices
  int px[8], py[8];
  for (int i = 0; i < 8; i++) {
    Vec3 rv = rotate(cube.vertices[i], rotation_x, rotation_y, rotation_z);
    project(rv, px[i], py[i]);
  }

  // Clear the display
  display.clearDisplay();
  
  // Draw cube edges
  for (int i = 0; i < 12; i++) {
    int x1 = px[cube.edges[i][0]];
    int y1 = py[cube.edges[i][0]];
    int x2 = px[cube.edges[i][1]];
    int y2 = py[cube.edges[i][1]];
    
    // Ensure coordinates are within screen bounds
    if (x1 >= 0 && x1 < SCREEN_WIDTH && y1 >= 0 && y1 < SCREEN_HEIGHT &&
        x2 >= 0 && x2 < SCREEN_WIDTH && y2 >= 0 && y2 < SCREEN_HEIGHT) {
      display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
    }
  }
  
  // Draw vertices as dots
  for (int i = 0; i < 8; i++) {
    if (px[i] >= 0 && px[i] < SCREEN_WIDTH && py[i] >= 0 && py[i] < SCREEN_HEIGHT) {
      // Draw a small cross for each vertex
      display.drawPixel(px[i], py[i], SSD1306_WHITE);
      if (px[i] > 0) display.drawPixel(px[i]-1, py[i], SSD1306_WHITE);
      if (px[i] < SCREEN_WIDTH-1) display.drawPixel(px[i]+1, py[i], SSD1306_WHITE);
      if (py[i] > 0) display.drawPixel(px[i], py[i]-1, SSD1306_WHITE);
      if (py[i] < SCREEN_HEIGHT-1) display.drawPixel(px[i], py[i]+1, SSD1306_WHITE);
    }
  }
  
  // Display sensor values in corner (optional - comment out for cleaner cube display)
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("G:");
  display.print((int)(gx_filtered/100));
  display.print(",");
  display.print((int)(gy_filtered/100));
  display.print(",");
  display.print((int)(gz_filtered/100));

  // Update the display
  display.display();

  // Debug output every 2 seconds
  if (millis() - lastDebug > 2000) {
    Serial.print("Accel: ");
    Serial.print(ax_filtered); Serial.print(", ");
    Serial.print(ay_filtered); Serial.print(", ");
    Serial.print(az_filtered);
    Serial.print(" | Gyro: ");
    Serial.print(gx_filtered); Serial.print(", ");
    Serial.print(gy_filtered); Serial.print(", ");
    Serial.print(gz_filtered);
    Serial.print(" | Rotation: ");
    Serial.print(rotation_x, 4); Serial.print(", ");
    Serial.print(rotation_y, 4); Serial.print(", ");
    Serial.println(rotation_z, 4);
    lastDebug = millis();
  }
  
  delay(50); // ~20 FPS
}