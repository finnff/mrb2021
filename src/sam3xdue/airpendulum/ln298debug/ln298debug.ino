const int dirControlPin = 6;
const int dir1Pin = 7;
const int dir2Pin = 8;
const int speedPin = 9;
const int potPin = A0;

void setup() {
  pinMode(dir1Pin, OUTPUT);
  pinMode(dir2Pin, OUTPUT);
  pinMode(speedPin, OUTPUT);
  pinMode(dirControlPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  int potValue = analogRead(potPin);
  int pwmValue = map(potValue, 0, 1023, 0, 255);
  int dirState = digitalRead(dirControlPin);  // Read direction control pin

  // Print relevant information to Serial
  Serial.print("Potentiometer Value: ");
  Serial.print(potValue);
  Serial.print(", PWM Value: ");
  Serial.print(pwmValue);
  Serial.print(", Direction: ");

  analogWrite(speedPin, pwmValue);  // Send PWM signal to L298N

  if (dirState == HIGH) {
    digitalWrite(dir1Pin, HIGH);  // Forward
    digitalWrite(dir2Pin, LOW);
    Serial.println("Forward");
  } else {
    digitalWrite(dir1Pin, LOW);   // Backward
    digitalWrite(dir2Pin, HIGH);
    Serial.println("Backward");
  }
  delay(10);  // Small delay for stability and readability of serial output
}
