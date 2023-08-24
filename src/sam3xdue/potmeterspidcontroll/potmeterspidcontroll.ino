#include <Wire.h>
#include <PCF8574.h>
#include <LiquidCrystal_I2C.h>

// Addresses for PCF8574
/* NXP PCF8574 all address pins pulled high */
PCF8574 pcf8574(0x3F);

// Analog pins for potentiometers
const int pot1Pin = A0;
const int pot2Pin = A1;
const int pot3Pin = A2;
const int pot4Pin = A3;

// Set the LCD address to 0x27 for a 16x2 display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pcf8574.begin();

 lcd.init();                      // Initialize the LCD
  lcd.backlight();                 // Turn on the backlight
  lcd.setCursor(0, 0);
  lcd.print("Hello,");             // Print on the first line
  lcd.setCursor(0, 1);
  lcd.print("Arduino!");  
}

void loop() {
  int pot1Value = readPot(pot1Pin, 0, 1);
  int pot2Value = readPot(pot2Pin, 2, 3);
  int pot3Value = readPot(pot3Pin, 4, 5);

  Serial.print("Pot1 Value: "); Serial.print(pot1Value);
  Serial.print(" Pot2 Value: "); Serial.print(pot2Value);
  Serial.print(" Pot3 Value: "); Serial.println(pot3Value);
  lcd.setCursor(0, 1);
  lcd.print(pot3Value);           // Print on the second line
  delay(500);  // Delay for readability
}

int readPot(int potPin, int sensitivityPin1, int sensitivityPin2) {
  bool s1 = pcf8574.read(sensitivityPin1);
  bool s2 = pcf8574.read(sensitivityPin2);

  int multiplier = getMultiplier(s1, s2);
  int potValue = analogRead(potPin);

  return potValue * multiplier;
  delay(1);
}

int getMultiplier(bool s1, bool s2) {
  if (s1 && s2) {
    return 0;  // Off
  } else if (s1 && !s2) {
    return 1;  // Low sensitivity
  } else if (!s1 && s2) {
    return 100;  // Medium sensitivity
  } else {  // !s1 && !s2
    return 1000;  // High sensitivity
  }
}



