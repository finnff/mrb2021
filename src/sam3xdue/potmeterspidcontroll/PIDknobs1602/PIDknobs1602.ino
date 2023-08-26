#include <Wire.h>
#include <PCF8574.h>
#include <LiquidCrystal_I2C.h>

// Addresses for PCF8574
PCF8574 pcf8574(0x3F);

// Analog pins for potentiometers
const int potPins[] = {A1, A0, A2, A3};  // A0 to A3

// Set the LCD address to 0x27 for a 16x2 display
LiquidCrystal_I2C lcd(0x27, 16, 2);

int mapSensitivity(bool s1, bool s2) {
  if (s1 && s2) {
    return 0;
  } else if (s1 && !s2) {
    return 1;
  } else if (!s1 && s2) {
    return 2;
  } else {
    return 3;
  }
}

int multipliers[] = {0,1,10,100};
char names[] = {'p','i','d','X'};

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pcf8574.begin();

  lcd.init();
  lcd.backlight();
  

lcd.setCursor(0, 0);  // Top left
lcd.print(names[0]);
lcd.print(": ");

lcd.setCursor(7, 0);  // Top right
lcd.print("|");
lcd.print(names[1]);
lcd.print(": ");

lcd.setCursor(0, 1);  // Bottom left
lcd.print(names[2]);
lcd.print(": ");


lcd.setCursor(7, 1); 
lcd.print("|");
lcd.setCursor(12, 1);  // Bottom right
lcd.print(names[3]);
lcd.print(": ");
  /* for (int i = 0; i < 4; i++) { */
    /* lcd.setCursor(i * 8, i / 2); */
    /* lcd.print(names[i]); */
    /* lcd.print(":"); */
  /* } */
}


void loop() {
  int sensitivityLevels[4] = {0, 0, 0,1};

  for (int i = 0; i < 8; i += 2) {
    bool s1 = pcf8574.read(i);
    bool s2 = pcf8574.read(i + 1);
    sensitivityLevels[i / 2] = mapSensitivity(s1, s2);
  }

  int MappedPotValues[4];
  int scaledPotValues[4];

  Serial.print("Values: ");
  
  for (int i = 0; i < 4; i++) {
    MappedPotValues[i] = map(analogRead(potPins[i]), 0, 1023, 0, 100);
    scaledPotValues[i] = MappedPotValues[i] * multipliers[sensitivityLevels[i]];
    // Print values to Serial
    Serial.print(names[i]);
    Serial.print(": ");
    Serial.print(scaledPotValues[i]);
    Serial.print(" (x");
    Serial.print(multipliers[sensitivityLevels[i]]);
    Serial.print(") ");
  }

  Serial.println(); // end of line after printing all values

  lcd.setCursor(2, 0);  // Top left value
  lcd.print(MappedPotValues[0]);
  lcd.setCursor(6, 0);  
  lcd.print(sensitivityLevels[0]);
  
  lcd.print("|");
  lcd.setCursor(10, 0);  // Top right value
  lcd.print(MappedPotValues[1]);
  lcd.setCursor(14, 0);  
  lcd.print(sensitivityLevels[1]);

  lcd.setCursor(2, 1);  // Bottom left value
  lcd.print(MappedPotValues[2]);
  lcd.setCursor(6, 1);  
  lcd.print(sensitivityLevels[2]);
  lcd.print("|");


  lcd.setCursor(14, 1);  // Bottom right value
  lcd.print(MappedPotValues[3]);

  delay(100);
}
