#include <Arduino.h>
#include "PotManager.h"
#include "LCD1602SlaveController.h"


int LCD_Ydisp = -1;
int SensitivityMultipliers[] = {0, 1, 10, 100};
PotManager PIDtuner(&LCD_Ydisp,SensitivityMultipliers);

int setPoint = 0;      // Desired angle (typically 0 for pendulum systems)

uint16_t Kp = 1;
uint16_t Ki = 1;
uint16_t Kd = 1;






const int SLAVE_ADDRESS = 0x0B; 
LCD1602SlaveController lcdSlaveController;

void setup() {
    Wire.begin(); // Join the bus as a master
    Serial.begin(9600);
    PIDtuner.begin();
    lcdSlaveController.begin1602slave(SLAVE_ADDRESS, "q:", "w:", "e:", "r:");
    delay(1000);
}

void loop() {
  potValue = analogRead(A6);           
  LCD_Ydisp = map(potValue, 0, 1023, 0, 100); 
  PIDtuner.updateAndDisplay();

  // Fetch the scaled values
  int values[4];
  PIDtuner.getScaledValues(values);
  Kp = values[0];
  Ki = values[1];
  Kd = values[2];
  setPoint = values[3];
  lcdSlaveController.update1602slave(Kp, Ki, Kd, LCD_Ydisp );
  
  

  delay(5);
}


void setMotorState(int pwmValue, bool forwardDirection) {
    // Set motor direction
    /* if (forwardDirection) { */
        /* digitalWrite(MOTOR_DIRECTION_IN1, HIGH); */
        /* digitalWrite(MOTOR_DIRECTION_IN2, LOW); */
    /* } else { */
        /* digitalWrite(MOTOR_DIRECTION_IN1, LOW); */
        /* digitalWrite(MOTOR_DIRECTION_IN2, HIGH); */
    /* } */
    // Set motor speed
    analogWrite(speedPin, pwmValue);

}


