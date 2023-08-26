#include <Arduino.h>
#include "PotManager.h"
#include "LCD1602SlaveController.h"
#include <Wire.h>

#define MOTOR_DIRECTION_IN1 7
#define MOTOR_DIRECTION_IN2 8
#define PWN_MOTOR_SPEED 9
#define SLAVE_ADDRESS 0x0B
#define PRINT_DEBUG false


uint16_t potValue;

int LCD_Ydisp = -1;
const int SensitivityMultipliers[] = {0, 1, 10, 1};
PotManager PIDtuner(&LCD_Ydisp, SensitivityMultipliers);

int setPoint = 0; // Desired position or angle
bool motorTurnDirection = false;

uint16_t Kp = 1;
uint16_t Ki = 1;
uint16_t Kd = 1;

int lastError = 0; // Store last error
int integral = 0; // Store integral term

unsigned long startTime;

LCD1602SlaveController lcdSlaveController;

void setup() {
  pinMode(MOTOR_DIRECTION_IN1, OUTPUT);
  pinMode(MOTOR_DIRECTION_IN2, OUTPUT);
  pinMode(PWN_MOTOR_SPEED, OUTPUT);
  Wire.begin(); // Join the bus as a master
  Serial.begin(9600);
  PIDtuner.begin();
  lcdSlaveController.begin1602slave(SLAVE_ADDRESS, "Er:", "OU:", "pw", "Hz");
  delay(1000);
  startTime = millis();  // Start time measurement
}

void loop() {
  potValue = analogRead(A6);
  LCD_Ydisp = map(potValue, 0, 1023, 0, 100);

  int error = setPoint - potValue; // Calculate the error

  integral += error; // Calculate integral

  int derivative = error - lastError; // Calculate derivative

  int output = Kp * error + Ki * integral + Kd * derivative; // PID formula

  lastError = error; // Update last error

  int pwmValue = constrain(output, 0, 255); // Constrain PID output to be within 0-255
  
  
  PIDtuner.updateAndDisplay();
  
  // Fetch the scaled values
  int values[4];
  PIDtuner.getScaledValues(values);
  Kp = values[0];
  Ki = values[1];
  Kd = values[2];
  setPoint = values[3]; //x


  unsigned long endTime = millis();  // End time measurement
  unsigned long elapsedTime = 1000/( endTime - startTime);  // Calculate elapsed time then conv to frequency 
  lcdSlaveController.update1602slave(error, output, pwmValue,elapsedTime);
  setMotorState(pwmValue, motorTurnDirection); // Set motor state based on PID output


  if(PRINT_DEBUG){
      Serial.print("T: " + String(elapsedTime) + "ms ");  // Print elapsed time in microseconds
Serial.println("X: "+ String(potValue)+  " SP: " + String(setPoint)  +" Er: " + String(error) + " I: " + String(integral) + " D: " + String(derivative) + " Out: " + String(output) + " PWM: " + String(pwmValue) +  " Kp: " + String(Kp) + " Ki: " + String(Ki) + " Kd: " + String(Kd));
 PIDtuner.debug_print();
  delay(5);
  }


  startTime = millis();  // Start time measurement
}

void setMotorState(int pwmValue, bool forwardDirection) {
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


