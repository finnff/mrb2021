#include <LiquidCrystal.h>
#include <Wire.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int SLAVE_ADDRESS = 0x0B; // Example I2C address, you can change this

void setup() {
  Wire.begin(SLAVE_ADDRESS);  
  Wire.onReceive(receiveEvent); 
  lcd.begin(16, 2);   
  lcd.setCursor(0, 0); // Set cursor to first position of first row
  lcd.print("waiting for i2c");

  lcd.setCursor(0, 1); // Set cursor to first position of first row
  lcd.print(SLAVE_ADDRESS);

}

void loop() {
  // The loop remains empty as actions are taken on I2C events
}

void receiveEvent(int numBytes) {
  char command = Wire.read();
  
  if (command == 'I') { // 'I' for init4
    char a[9], b[9], c[9], d[9]; 
    Wire.readBytes(a, 8);
    Wire.readBytes(b, 8);
    Wire.readBytes(c, 8);
    Wire.readBytes(d, 8);
    a[8] = b[8] = c[8] = d[8] = '\0'; // Null termination
    init4(a, b, c, d);
  } 
  else if (command == 'U') { // 'U' for update4
    String e, f, g, h;
    e = Wire.readStringUntil('\0');
    f = Wire.readStringUntil('\0');
    g = Wire.readStringUntil('\0');
    h = Wire.readStringUntil('\0');
    update4(e, f, g, h);
  }
}

void init4(const char* a, const char* b, const char* c, const char* d) {

  lcd.setCursor(0, 0); // Set cursor to first position of first row
  lcd.print(a);

  lcd.setCursor(8, 0); // Set cursor to ninth position of first row
  lcd.print(b);

  lcd.setCursor(0, 1); // Set cursor to first position of second row
  lcd.print(c);

  lcd.setCursor(8, 1); // Set cursor to ninth position of second row
  lcd.print(d);
}

void update4(String &e, String &f, String &g, String &h) {
  lcd.setCursor(3, 0); // Set cursor to fourth position of first row
  lcd.print(e);

  lcd.setCursor(11, 0); // Set cursor to twelfth position of first row
  lcd.print(f);

  lcd.setCursor(3, 1); // Set cursor to fourth position of second row
  lcd.print(g);

  lcd.setCursor(11, 1); // Set cursor to twelfth position of second row
  lcd.print(h);
}

