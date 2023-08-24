#include <Arduino.h>
/* #include "./rotaryencoderLib/Encoder.h" */
#include "Encoder.h"
#include "PotManager.h"

// Define the GPIO pins for the L298N IN1 and IN2 for PWM
#define MOTOR_IN1_PIN 32
#define MOTOR_IN2_PIN 30

// Define the GPIO pins for the L298N IN1 and IN2 for Direction
#define MOTOR_DIRECTION_IN1 28
#define MOTOR_DIRECTION_IN2 26

#define ROTARY_ENCODER_A_PIN 40
#define ROTARY_ENCODER_B_PIN 38
#define ROTARY_ENCODER_C_PIN 36

#define POTMETER_ADC_IN_PIN A1

#define PWM_PERIOD 1000 // 1kHz frequency
//
#define ADC_MIN 0
#define ADC_MAX 1023
#define TOTAL_ANGLE_RANGE 300

// Global variables
// start rotary ROTARY_ENCODER
//



int SensitivityMultipliers[] = {0, 1, 10, 100};
PotManager potManager(SensitivityMultipliers);

Encoder myEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN);
long oldPosition = -999;
bool lastButtonState = LOW;
bool currentDirection = true;
uint32_t motor_speed = 0;

uint16_t potValue = 0;
bool calibrated = false;  // You can initialize this according to your needs.
/* int ANGLE_MIN_INT = -150; // Placeholder. Adjust this value as needed. */
int ANGLE_MIN_INT = (int)(-0.5* TOTAL_ANGLE_RANGE);
/* int ANGLE_MAX_INT = 150;  // Placeholder. Adjust this value as needed. */
int ANGLE_MAX_INT = (int)(0.5* TOTAL_ANGLE_RANGE);

int speedIncrement = 25;
int angle_int;
uint16_t ADC_EQUILLIBRIUM = ADC_MAX / 2; // Assuming equilibrium at the middle point of ADC

int INTEGRAL_LIMIT = 10000;


// PID Variables
double setPoint = 0;      // Desired angle (typically 0 for pendulum systems)
/* double Kp = 0.05;          // Proportional gain (Placeholder, adjust as necessary) */
/* double Ki = 0.1;          // Integral gain (Placeholder, adjust as necessary) */
/* double Kd = 0.01;         // Derivative gain (Placeholder, adjust as necessary) */

double Kp = 200.8;
double Ki = 50;
double Kd = 50;




double error = 0;
double previousError = 0;
double integral = 0;
double previous_error;
double derivative = 0;
double PIDOutput = 0;


int setPointAngle = 0;  // Desired setpoint in terms of angle (-150 to 150)  AKA DEFFAULT SWING
const int ROTARY_ENCODER_MAX = 100;
/* const int MOTOR_SPEED_MAX = 4095; */
const int MOTOR_SPEED_MAX = 2000;
const int PID_MIN_OUTPUT = 2000;

// Function to set motor direction
void set_motor_direction(bool forward)
{
    if (forward) {
        digitalWrite(MOTOR_DIRECTION_IN1, HIGH);
        digitalWrite(MOTOR_DIRECTION_IN2, LOW);
    } else {
        digitalWrite(MOTOR_DIRECTION_IN1, LOW);
        digitalWrite(MOTOR_DIRECTION_IN2, HIGH);
    }
}


float ADCtoAngle(uint16_t adcValue) {
    float ratio;
    if (adcValue <= ADC_EQUILLIBRIUM) {
        ratio = (float)adcValue / ADC_EQUILLIBRIUM - 1;
    } else {
        ratio = (float)(adcValue - ADC_EQUILLIBRIUM) / (ADC_MAX - ADC_EQUILLIBRIUM);
    }
    return ANGLE_MAX_INT * ratio;
}




void setup()
{

    potManager.begin();
    Serial.begin(9600);


    pinMode(MOTOR_DIRECTION_IN1, OUTPUT);
    pinMode(MOTOR_DIRECTION_IN2, OUTPUT);
    pinMode(MOTOR_IN1_PIN, OUTPUT);
    pinMode(MOTOR_IN2_PIN, OUTPUT);
    /* pinMode(ROTARY_ENCODER_A_PIN, INPUT); */
    /* pinMode(ROTARY_ENCODER_B_PIN, INPUT); */

    pinMode(ROTARY_ENCODER_C_PIN, INPUT_PULLUP); // Assuming the button is active LOW

    // The Due doesn't have a direct API for setting PWM frequency,
    // so the following lines are a common way of setting the PWM frequency for pins 5 and 6
    analogWrite(MOTOR_IN1_PIN, 0);
    analogWrite(MOTOR_IN2_PIN, 0);
    /* analogWriteResolution(12); // 0 to 4095, where 4095 is 100% duty cycle */
    /* Serial.println("Arduino DUE Started"); */

    // Calibration logic (Adapted from the old code)
    uint16_t potValue = analogRead(A1);
    ADC_EQUILLIBRIUM = potValue;

    if (ADC_EQUILLIBRIUM >= 0) { // Replacing the ESP_OK check as the Arduino analogRead doesn't return error codes
        int adc_range_max = ADC_MAX - ADC_EQUILLIBRIUM;

        // Calculate proportion of the maximum angle we can achieve based on available ADC range
        float proportion_max = (float)adc_range_max / (ADC_MAX / 2); 

        // Calculate ANGLE_MAX_INT based on proportion of maximum possible
        ANGLE_MAX_INT = (int)(TOTAL_ANGLE_RANGE / 2.0 * proportion_max);

        // Calculate ANGLE_MIN_INT based on the remaining angle from total
        ANGLE_MIN_INT = TOTAL_ANGLE_RANGE - ANGLE_MAX_INT;

        // Since it should be symmetric about zero and negative, invert it
        ANGLE_MIN_INT = -ANGLE_MIN_INT;

        calibrated = true;
        printf("Calibration complete. ADC_EQUILLIBRIUM: %d, ANGLE_MIN_INT: %d, ANGLE_MAX_INT: %d\n", ADC_EQUILLIBRIUM, ANGLE_MIN_INT, ANGLE_MAX_INT);



        setPointAngle = 20;  // Default to a 20-degree swing
        setPoint = ADC_EQUILLIBRIUM + (ADC_EQUILLIBRIUM / ANGLE_MAX_INT) * setPointAngle;
    }
}


void loop()
{

    potManager.updateAndDisplay();
    // 1. Read the Input
    long newPosition = myEncoder.read();
    potValue = analogRead(A1);

    angle_int = (int)(ADCtoAngle(potValue));

    // 2. PID Controller Calculation
    error = setPoint - angle_int;
    integral += error;
    integral = constrain(integral, -INTEGRAL_LIMIT, INTEGRAL_LIMIT);
    derivative = error - previousError;
    PIDOutput = Kp * error + Ki * integral + Kd * derivative;

    // Set PID output minimum threshold
    if (abs(PIDOutput) < PID_MIN_OUTPUT && PIDOutput != 0) {
        PIDOutput = (PIDOutput > 0) ? PID_MIN_OUTPUT : -PID_MIN_OUTPUT;
    }



/*     if(newPosition != oldPosition){ */
/* // Increment or decrement setPoint based on encoder direction */
/*         setPoint += (newPosition > oldPosition) ? (ADC_MAX - ADC_EQUILLIBRIUM) / ANGLE_MAX_INT :  */
/*                                                  -(ADC_EQUILLIBRIUM - ADC_MIN) / ANGLE_MIN_INT; */
/**/
/*         // Constrain the ADC value between ADC_MIN and ADC_MAX */
/*         setPoint= constrain(setPoint, ADC_MIN, ADC_MAX); */
/**/
/*         // Convert the setpoint ADC to its corresponding angle */
/*          setPointAngle = ADCtoAngle(setPoint); */
/*         oldPosition = newPosition; */
/* } */
/**/
/**/

    if(newPosition != oldPosition){
        // Increment or decrement setPoint based on encoder direction
        setPoint += (newPosition > oldPosition) ? (ADC_MAX - ADC_EQUILLIBRIUM) / ANGLE_MAX_INT : 
                                                 -(ADC_EQUILLIBRIUM - ADC_MIN) / ANGLE_MIN_INT;
        
        // Constrain the ADC value between ADC_MIN and ADC_MAX
        setPoint = constrain(setPoint, ADC_MIN, ADC_MAX);
        
        // Convert the setpoint ADC to its corresponding angle
        setPointAngle = ADCtoAngle(setPoint);
        oldPosition = newPosition;
    }

    // 3. Motor speed and direction
    /* motor_speed = map(newPosition, 0, ROTARY_ENCODER_MAX, 0, MOTOR_SPEED_MAX); */
    /* motor_speed = constrain(motor_speed, 0, MOTOR_SPEED_MAX); */
    if(newPosition !=oldPosition){
        // Increment or decrement setPointAngle based on encoder direction

        setPointAngle += (newPosition > oldPosition) ? 1 : -1;

        // Constrain the angle between ANGLE_MIN and ANGLE_MAX
        setPointAngle = constrain(setPointAngle, ANGLE_MIN_INT, ANGLE_MAX_INT);
        setPoint = ADC_EQUILLIBRIUM + (ADC_EQUILLIBRIUM /ANGLE_MAX_INT) * setPointAngle;
        oldPosition   = newPosition;
    }

    bool currentButtonState = digitalRead(ROTARY_ENCODER_C_PIN);
    if (currentButtonState == LOW && lastButtonState == HIGH) {
        currentDirection = !currentDirection;
        set_motor_direction(currentDirection); // Assuming you've defined this function elsewhere
    }
    lastButtonState = currentButtonState;

    // 4. Print the Status
    String statusMessage = "ADC: " + String(potValue);
    statusMessage += ", Angle: " + String(angle_int) + "." + String(abs(angle_int) % 100);
    statusMessage += " [" + String(ANGLE_MIN_INT + abs(angle_int)) + ", " + String(ANGLE_MAX_INT - angle_int) + "]" + ", Rotary: " + String(newPosition);
    statusMessage += ", Motor: " + String(motor_speed) + ", Direction: " + (currentDirection ? "disabled" : "ENABLED");
    statusMessage += ", Setpoint: " + String(setPoint) + "Spang:("+ String(setPointAngle) + ")";
    statusMessage += ", ERR: " + String(error);
    statusMessage += ", INT: " + String(integral);
    statusMessage += ", DER: " + String(derivative);
    statusMessage += ", OUT: " + String(PIDOutput);
    Serial.println(statusMessage);

    // 5. Control Motor using PWM
    motor_speed = constrain(PIDOutput , 0, MOTOR_SPEED_MAX);
    analogWrite(MOTOR_IN1_PIN, motor_speed);
    analogWrite(MOTOR_IN2_PIN, MOTOR_SPEED_MAX - motor_speed);

    // Store error for next iteration
    previousError = error;
}
