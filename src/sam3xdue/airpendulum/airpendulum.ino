#include "LCD1602SlaveController.h"
#include "PotManager.h"
#include <Arduino.h>
#include <Wire.h>

#define MOTOR_DIRECTION_IN1 7
#define MOTOR_DIRECTION_IN2 8
#define PWN_MOTOR_SPEED 9
#define SLAVE_ADDRESS 0x0B
#define PRINT_DEBUG false
#define MAX_INTEGRAL 1000 // Integral windup limits
#define MIN_INTEGRAL -1000


int LCD_Ydisp = -1;
const int SensitivityMultipliers[] = {1, 1, 1};
PotManager PIDtuner(&LCD_Ydisp, SensitivityMultipliers);

int setPoint = 0; // Desired position or angle
bool motorTurnDirection = false;

uint16_t potValue;

long Kp = 1; // Initial PID gains
long Ki = 1;
long Kd = 1;

long integral = 0; // Integral term
int lastError = 0; // Last error

LCD1602SlaveController lcdSlaveController;


void setup() {
  pinMode(2, OUTPUT);  // Initialize D2 as an output
  digitalWrite(2, HIGH);  // Initially set to HIGH (Assuming HIGH is normal operation)
  pinMode(MOTOR_DIRECTION_IN1, OUTPUT);
  pinMode(MOTOR_DIRECTION_IN2, OUTPUT);
  pinMode(PWN_MOTOR_SPEED, OUTPUT);
  Wire.begin(); // Join the bus as a master
  Serial.begin(9600);
  PIDtuner.begin();
  lcdSlaveController.begin1602slave(SLAVE_ADDRESS, "Er:", "OU:", "pw", "ms");
  delay(1000);
}

void loop() {
  potValue = analogRead(A6);
  int error = setPoint - potValue;

  integral += error;

  if (integral > MAX_INTEGRAL) integral = MAX_INTEGRAL; // Anti-windup
  if (integral < MIN_INTEGRAL) integral = MIN_INTEGRAL;

  int derivative = error - lastError;


   long output = (Kp * error + Ki * integral + Kd * derivative) / 10;  // Scaling factor of 100

  int pwmValue = abs(constrain(output, -255, 255));
  motorTurnDirection = (output < 0);

  // Other code remains the same
  LCD_Ydisp = map(potValue, 0, 1023, 0, 100);
  PIDtuner.updateAndDisplay();

  int values[4];
  PIDtuner.getScaledValues(values);
  Kp = values[0];
  Ki = values[1];
  Kd = values[2];
  setPoint = values[3];

  lcdSlaveController.update1602slave(error, output, pwmValue, 0); // Removed dt
  
  Serial.println("X: " + String(potValue) + " SP: " + String(setPoint) + " Er: " + String(error) + " I: " + String(integral) + " D: " + String(derivative) + " Out: " + String(output) + " PWM: " + String(pwmValue) + " Kp: " + String(Kp) + " Ki: " + String(Ki) + " Kd: " + String(Kd));
if (potValue < 300 || potValue > 800) {
   Serial.println("TRIPPED RESET ANGLE TOO HIGH") ;
    Serial.println("X: " + String(potValue) + " SP: " + String(setPoint) +  " Kp: " + String(Kp) + " Ki: " + String(Ki) + " Kd: " + String(Kd));
    derivative, pwmValue, derivative ,error, output = 0;
    setMotorState(0, motorTurnDirection); // Set motor state based on PID output
    delay(5000);
 }

  setMotorState(pwmValue, motorTurnDirection); // Set motor state based on PID output

  lastError = error;
}

/* int LCD_Ydisp = -1; */
/* const int SensitivityMultipliers[] = { 0, 1, 10, 1 }; */
/* PotManager PIDtuner(&LCD_Ydisp, SensitivityMultipliers); */
/**/
/* int setPoint = 0; // Desired position or angle */
/* bool motorTurnDirection = false; */
/**/
/* uint16_t potValue; */
/**/
/* long Kp = 100; // Initial PID gains */
/* long Ki = 100; */
/* long Kd = 100; */
/**/
/* long integral = 0; // Integral term */
/* int lastError = 0; // Last error */
/**/
/* unsigned long startTime; */
/**/
/* LCD1602SlaveController lcdSlaveController; */
/**/
/* #define MAX_INTEGRAL 1000000 // Integral windup limits */
/* #define MIN_INTEGRAL -1000000 */
/**/
/* void setup() { */
/*     pinMode(MOTOR_DIRECTION_IN1, OUTPUT); */
/*     pinMode(MOTOR_DIRECTION_IN2, OUTPUT); */
/*     pinMode(PWN_MOTOR_SPEED, OUTPUT); */
/*     Wire.begin(); // Join the bus as a master */
/*     Serial.begin(9600); */
/*     PIDtuner.begin(); */
/*     lcdSlaveController.begin1602slave(SLAVE_ADDRESS, "Er:", "OU:", "pw", "ms"); */
/*     delay(1000); */
/*     startTime = millis(); // Start time measurement */
/* } */
/**/
/* unsigned long lastTime = 0; */
/**/
/* void loop() { */
/*     unsigned long currentTime = millis(); */
/*     int dt = currentTime - lastTime; // delta time */
/**/
/*     if (dt <= 0) { // Avoid division by zero */
/*         return; */
/*     } */
/**/
/*     potValue = analogRead(A6); */
/*     int error = setPoint - potValue; */
/**/
/*     integral += error * dt; */
/**/
/*     if (integral > MAX_INTEGRAL) integral = MAX_INTEGRAL; // Anti-windup */
/*     if (integral < MIN_INTEGRAL) integral = MIN_INTEGRAL; */
/**/
/*     int derivative = (error - lastError) / dt; */
/**/
/*     long output = (Kp * error + Ki * integral + Kd * derivative) ; // Your PID calculation here */
/**/
/*     int pwmValue = abs(constrain(output, -255, 255)); */
/*     motorTurnDirection = (output < 0); */
/**/
/*     // Other code remains the same */
/*     LCD_Ydisp = map(potValue, 0, 1023, 0, 100); */
/*     PIDtuner.updateAndDisplay(); */
/**/
/*     // Fetch the scaled values */
/*     int values[4]; */
/*     PIDtuner.getScaledValues(values); */
/*     Kp = values[0]; */
/*     Ki = values[1]; */
/*     Kd = values[2]; */
/*     setPoint = values[3]; */
/**/
/*     lcdSlaveController.update1602slave(error, output, pwmValue, dt); */
/**/
/*     Serial.println("dt: " + String("DONT") + " X: " + String(potValue) + " SP: " + String(setPoint) + " Er: " + String(error) + " I: " + String(integral) + " D: " + String(derivative) + " Out: " + String(output) + " PWM: " + String(pwmValue) + " Kp: " + String(Kp) + " Ki: " + String(Ki) + " Kd: " + String(Kd)); */
/**/
/*     setMotorState(pwmValue, motorTurnDirection); // Set motor state based on PID output */
/**/
/*     lastTime = currentTime; */
/*     lastError = error; */
/* } */
/**/


void setMotorState(int pwmValue, bool forwardDirection)
{
    // Set motor direction
    if (forwardDirection) {
        digitalWrite(MOTOR_DIRECTION_IN1, HIGH);
        digitalWrite(MOTOR_DIRECTION_IN2, LOW);
    } else {
        digitalWrite(MOTOR_DIRECTION_IN1, LOW);
        digitalWrite(MOTOR_DIRECTION_IN2, HIGH);
    }
    analogWrite(PWN_MOTOR_SPEED, pwmValue);
}
