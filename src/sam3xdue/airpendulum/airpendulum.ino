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

#define ENABLE_SAVGOL_FILTER true // enable or disable filtering
#define WINDOW_SIZE 5 // Define the savgol window size
#define SAV_GOL_POLYNOMINAL 1 // set polynomial to 0, 1 or 2 
//polynomial 2/3 require esp32 or Due
/* requries calculations on 32bit signed longs), wont work on nano/uno  */

int LCD_Ydisp = -1; // pass rod angle reference into first display
const int SensitivityMultipliers[] = { 1, 1, 1 };
PotManager PIDtuner(&LCD_Ydisp, SensitivityMultipliers);

int setPoint = 0; // Desired position or angle
bool motorTurnDirection = false;

uint16_t RawPotValue = 0;
uint16_t potValue = 0;

// static array and index for our savgol implementation.
static uint16_t SGdata[WINDOW_SIZE] = { 0 }; // Static array to hold window of data
static uint8_t SGdataIndex = 0; // Index to keep track of the oldest data

long Kp = 1; // Initial PID gains
long Ki = 1;
long Kd = 1;

long integral = 0; // Integral term
int lastError = 0; // Last error

LCD1602SlaveController lcdSlaveController;

long startTime = 0;
long elapsedTime = 0;

void setup()
{
    pinMode(2, OUTPUT); // Initialize D2 as an output
    digitalWrite(2, HIGH); // Initially set to HIGH (Assuming HIGH is normal operation)
    pinMode(MOTOR_DIRECTION_IN1, OUTPUT);
    pinMode(MOTOR_DIRECTION_IN2, OUTPUT);
    pinMode(PWN_MOTOR_SPEED, OUTPUT);
    Wire.begin(); // Join the bus as a master
    Serial.begin(9600);
    PIDtuner.begin();
    lcdSlaveController.begin1602slave(SLAVE_ADDRESS, "Er:", "OU:", "pw", "ms");
    delay(1000);
    // Initialize the savgol data array by reading RawPotValue for WINDOW_SIZE times (or first 5 readings will be wrong)
    if (ENABLE_SAVGOL_FILTER) {
        for (uint8_t i = 0; i < WINDOW_SIZE; i++) {
            RawPotValue = analogRead(A6);
            SGdata[i] = RawPotValue;
            delay(10);
        }
    }
}

void loop()
{

    startTime = millis(); // Start time measurement
    RawPotValue = analogRead(A6); // Raw Unfiltered ADC value

    if (ENABLE_SAVGOL_FILTER) {
        potValue = savGolayFilter(RawPotValue);
    } else {
        potValue = RawPotValue;
    }

    // pid code
    int error = setPoint - potValue;
    integral += error;
    if (integral > MAX_INTEGRAL)
        integral = MAX_INTEGRAL; // Anti-windup
    if (integral < MIN_INTEGRAL)
        integral = MIN_INTEGRAL;
    int derivative = error - lastError;
    long output = (Kp * error + Ki * integral + Kd * derivative) / 10; // Scaling factor of 10

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

    lcdSlaveController.update1602slave(error, output, pwmValue, elapsedTime);

    /* Serial.println("X: " + String(potValue) + " SP: " + String(setPoint) + " Er: " + String(error) + " I: " + String(integral) + " D: " + String(derivative) + " Out: " + String(output) + " PWM: " + String(pwmValue) + " Kp: " + String(Kp) + " Ki: " + String(Ki) + " Kd: " + String(Kd)); */
    // Serial printing this will sometimes break code; too much data on one line, only showing non LCD screen values:
    Serial.println("rX: " + String(potValue) + " rySP: " + String(setPoint) + " I: " + String(integral) + " D: " + String(derivative));
    if (RawPotValue < 300 || RawPotValue > 800) {
        Serial.println("TRIPPED RESET ANGLE TOO HIGH");
        Serial.println(String(RawPotValue) + " RAW  X: " + String(potValue) + " SP: " + String(setPoint) + " Kp: " + String(Kp) + " Ki: " + String(Ki) + " Kd: " + String(Kd));
        integral = 0;
        lastError = 0; // Last error
        setMotorState(0, motorTurnDirection); // Set motor state based on PID output
        delay(5000);
    }
    long endTime = millis(); // End time measurement
    elapsedTime = endTime - startTime; //
    setMotorState(pwmValue, motorTurnDirection); // Set motor state based on PID output

    lastError = error;
}

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

#if SAV_GOL_POLYNOMINAL == 2
  #if WINDOW_SIZE == 3
    int coefficients[] = {1, 3, 1};
    int coefficientDivider = 5;
  #elif WINDOW_SIZE == 5
    int coefficients[] = {-9, 34, 49, 34, -9};
    int coefficientDivider = 135;
  #elif WINDOW_SIZE == 7
    int coefficients[] = {-10, 14, 29, 33, 29, 14, -10};
    int coefficientDivider = 139;
  #elif WINDOW_SIZE == 9
    int coefficients[] = {-9, 6, 17, 23, 26, 23, 17, 6, -9};
    int coefficientDivider = 136;
  #elif WINDOW_SIZE == 11
    int coefficients[] = {-8, 2, 10, 16, 20, 21, 20, 16, 10, 2, -8};
    int coefficientDivider = 133;
  #elif WINDOW_SIZE == 13
    int coefficients[] = {-8, 0, 6, 11, 15, 17, 17, 17, 15, 11, 6, 0, -8};
    int coefficientDivider = 131;
  #endif
#endif

int savGolayFilter(uint16_t new_data)
{
    SGdata[SGdataIndex] = new_data;
    SGdataIndex = (SGdataIndex + 1) % WINDOW_SIZE;

    uint16_t returnValue = 0;

#if SAV_GOL_POLYNOMINAL == 0
    // For polynomial order 0, the filter is just an average
    uint16_t sum = 0;
    for (uint8_t i = 0; i < WINDOW_SIZE; i++) {
        sum += SGdata[i];
    }
    returnValue = sum / WINDOW_SIZE;

#elif SAV_GOL_POLYNOMINAL == 1
    // For polynomial order 1, it's similar to a linear regression
    long sumY = 0;
    int sumX = 0;
    long sumXY = 0;
    int sumX2 = 0;
    for (uint8_t i = 0; i < WINDOW_SIZE; i++) {
        sumY += SGdata[i];
        sumX += i;
        sumXY += i * SGdata[i];
        sumX2 += i * i;
    }
    int slope = (WINDOW_SIZE * sumXY - sumX * sumY) / (WINDOW_SIZE * sumX2 - sumX * sumX);
    int intercept = (sumY - slope * sumX) / WINDOW_SIZE;
    returnValue = intercept + slope * (WINDOW_SIZE - 1);



#elif SAV_GOL_POLYNOMINAL == 2
    long filteredValue = 0;
    for (uint8_t i = 0; i < WINDOW_SIZE; i++) {
        int term = coefficients[i] * SGdata[(SGdataIndex + i) % WINDOW_SIZE];
        filteredValue += term;
    }

    filteredValue /= coefficientDivider;

    if (filteredValue < 0)
        filteredValue = 0;
    if (filteredValue > 1023)
        filteredValue = 1023;

    returnValue = filteredValue;
#endif

    //using int's instead of floats sometimes gets weird overflow underflows?
    //we could change slope/intercept but at the cost of runtime
    //just ignoring filtering if this happens and returning windowAVG (~<1% of times)
    if (returnValue < 0 || returnValue > 1023) {
        uint16_t fsum = 0;
        for (uint8_t i = 0; i < WINDOW_SIZE; i++) {
            fsum += SGdata[i];
        }
        return fsum / WINDOW_SIZE;
    } else {
        return returnValue;
    }
}


