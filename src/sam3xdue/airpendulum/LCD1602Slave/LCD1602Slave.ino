#include <LiquidCrystal.h>
#include <Wire.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int SLAVE_ADDRESS = 0x0B; // Example I2C address, you can change this
bool inited = false;

void setup() {
  Serial.begin(9600);  // Start the serial communication
  Wire.begin(SLAVE_ADDRESS);  
  Wire.onReceive(receiveEvent); 
  lcd.begin(16, 2); 

  // Display message to show it's waiting for an I2C signal
  lcd.setCursor(0, 0);
  lcd.print("Waiting for I2C");
  lcd.setCursor(0, 1);
  lcd.print("Addr: 0x");
  lcd.print(String(SLAVE_ADDRESS, HEX));

  /* Serial.println("Slave setup done. Waiting for I2C signals."); */
}

void loop() {
  // The loop remains empty as actions are taken on I2C events
}

void receiveEvent(int numBytes) {
  char command = Wire.read();
    if (!inited){
    lcd.clear();
    init4("q:", "w:", "e:", "r:");
    inited=true;
    delay(1000);
    }
  /* Serial.print("Received command: "); */
  /* Serial.println(command); */

  if (command == 'I') { // 'I' for init4
    lcd.clear();
    char a[3], b[3], c[3], d[3]; 
    Wire.readBytes(a, 2);
    Wire.readBytes(b, 2);
    Wire.readBytes(c, 2);
    Wire.readBytes(d, 2);
    a[2] = b[2] = c[2] = d[2] = '\0'; // Null termination
    init4(a, b, c, d);
  } 
 else if (command == 'U') { // 'U' for update4
    char e[5], f[5], g[5], h[5];
    Wire.readBytes(e, 4);
    Wire.readBytes(f, 4);
    Wire.readBytes(g, 4);
    Wire.readBytes(h, 4);
    e[4] = f[4] = g[4] = h[4] = '\0'; // Null termination
    update4(e, f, g, h);
  }
}

void init4(const char* a, const char* b, const char* c, const char* d) {
  /* Serial.println("Initializing LCD..."); */
  
  lcd.begin(16, 2);  // Initialize the LCD for 16x2 characters

  lcd.setCursor(0, 0); // Set cursor to first position of first row
  lcd.print(a);

  lcd.setCursor(8, 0); // Set cursor to ninth position of first row
  lcd.print(b);

  lcd.setCursor(0, 1); // Set cursor to first position of second row
  lcd.print(c);

  lcd.setCursor(8, 1); // Set cursor to ninth position of second row
  lcd.print(d);

  /* Serial.println("Initialization done."); */
}

/* void update4(const int* e, const int* f, const int* g, const int* h) { */
void update4(const char* e, const char* f, const char* g, const char* h) {
  /* Serial.println("Updating LCD..."); */

  lcd.setCursor(3, 0); // Set cursor to fourth position of first row
  lcd.print(e);

  lcd.setCursor(11, 0); // Set cursor to twelfth position of first row
  lcd.print(f);

  lcd.setCursor(3, 1); // Set cursor to fourth position of second row
  lcd.print(g);

  lcd.setCursor(11, 1); // Set cursor to twelfth position of second row
  lcd.print(h);

  /* Serial.println("Update done."); */
}
