
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width
#define SCREEN_HEIGHT 64  // OLED display height

// Define the I2C address (typically it's 0x3C or 0x3D)
#define SSD1306_I2C_ADDRESS 0x3C 
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  if(!display.begin(SSD1306_I2C_ADDRESS, OLED_RESET)) {
    // If the display doesn't start, loop forever
    while(1);
  }

  display.display();  // Show the Adafruit splash screen (can be commented out if not desired)
  delay(2000);  // Wait for 2 seconds

  display.clearDisplay();  // Clear the buffer

  // Draw text
  display.setTextSize(1);      // Text size 1
  display.setTextColor(SSD1306_WHITE); // Set color
  display.setCursor(0,0);      // Start at top-left
  display.print(F("Hello, OLED!"));
  display.display();  // Refresh the display
}

void loop() {
  // Nothing here for this demo
}



