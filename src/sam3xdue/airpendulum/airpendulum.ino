#include <Arduino.h>
#include "PotManager.h"
#include "LCD1602SlaveController.h"

#define MOTOR_DIRECTION_IN1 7
#define MOTOR_DIRECTION_IN2  8
#define PWN_MOTOR_SPEED  9
#define SLAVE_ADDRESS 0x0B 

uint16_t potValue;

int LCD_Ydisp = -1;
const int SensitivityMultipliers[] = {0, 1, 10, 100};
PotManager PIDtuner(&LCD_Ydisp,SensitivityMultipliers);

int setPoint = 0;      // Desired angle (typically 0 for pendulum systems)
bool motorTurnDirection = false;

uint16_t Kp = 1;
uint16_t Ki = 1;
uint16_t Kd = 1;






LCD1602SlaveController lcdSlaveController;

void setup() {
 pinMode(MOTOR_DIRECTION_IN1, OUTPUT);
  pinMode(MOTOR_DIRECTION_IN2, OUTPUT);
  pinMode(PWN_MOTOR_SPEED, OUTPUT);
    Wire.begin(); // Join the bus as a master
    Serial.begin(9600);
    PIDtuner.begin();
    lcdSlaveController.begin1602slave(SLAVE_ADDRESS, "q:", "w:", "e:", "M%:");
    delay(1000);
}

void loop() {
  potValue = analogRead(A6);           
  int pwmValue = map(potValue, 0, 1023, 0, 255);
  LCD_Ydisp = map(potValue, 0, 1023, 0, 100); 
  PIDtuner.updateAndDisplay();

  // Fetch the scaled values
  int values[4];
  PIDtuner.getScaledValues(values);
  Kp = values[0];
  Ki = values[1];
  Kd = values[2];
  setPoint = values[3];
  if(setPoint>=50){
        motorTurnDirection = false;
    }else{
            motorTurnDirection=true;
        }
  lcdSlaveController.update1602slave(Kp, Ki,setPoint , LCD_Ydisp );
  
setMotorState(pwmValue,motorTurnDirection);
  

  delay(5);
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


