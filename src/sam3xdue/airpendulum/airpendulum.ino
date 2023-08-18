#include <Arduino.h>
/* #include "./rotaryencoderLib/Encoder.h" */
#include "Encoder.h"

// Define the GPIO pins for the L298N IN1 and IN2 for PWM
#define MOTOR_IN1_PIN 4
#define MOTOR_IN2_PIN 5

// Define the GPIO pins for the L298N IN1 and IN2 for Direction
#define MOTOR_DIRECTION_IN1 6
#define MOTOR_DIRECTION_IN2 7

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

Encoder myEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN);
long oldPosition = -999;
bool lastButtonState = LOW;
bool currentDirection = true;
uint32_t motor_speed = 0;

uint16_t potValue = 0;
bool calibrated = false;  // You can initialize this according to your needs.
int ANGLE_MIN_INT = -150; // Placeholder. Adjust this value as needed.
int ANGLE_MAX_INT = 150;  // Placeholder. Adjust this value as needed.

int speedIncrement = 25;
int angle_int;
uint16_t ADC_EQUILLIBRIUM = ADC_MAX / 2; // Assuming equilibrium at the middle point of ADC
int angle_range_max = 100;
int angle_range_min = -100;


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

void setup()
{
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
    analogWriteResolution(12); // 0 to 4095, where 4095 is 100% duty cycle


    //calibration
    uint16_t potValue = analogRead(A1);
    Serial.println("Arduino DUE Started");
        if (potValue <= ADC_EQUILLIBRIUM) {
        float temp_angle = -150 + (float)potValue / ADC_EQUILLIBRIUM * 150;
        angle_int = (int)(temp_angle);
    } else {
    float temp_angle = ((float)(potValue - ADC_EQUILLIBRIUM) / ADC_EQUILLIBRIUM) * 150;
    angle_int = (int)(temp_angle);
    }

    if (calibrated) {
        // Print calibrated values
        printf("Calibrated: true\n");
        printf("ADC_EQUILLIBRIUM: %d, ANGLE_MIN_INT: %d, ANGLE_MAX_INT: %d\n",
        ADC_EQUILLIBRIUM, ANGLE_MIN_INT, ANGLE_MAX_INT);
    } else {
        // Print information about the calibration process
        printf("Calibrating Angle, Equilibrium, and Range based on Neutral pendulum.\n");
        printf("ADC_EQUILLIBRIUM: %d\n", ADC_EQUILLIBRIUM);
    }

}


void loop()
{
    // 1. Read the Input
    long newPosition = myEncoder.read();
    potValue = analogRead(A1);

    if (potValue <= ADC_EQUILLIBRIUM) {
        float temp_angle = -150 + (float)potValue / ADC_EQUILLIBRIUM * 150;
        angle_int = (int)(temp_angle);
    } else {
    float temp_angle = ((float)(potValue - ADC_EQUILLIBRIUM) / ADC_EQUILLIBRIUM) * 150;
    angle_int = (int)(temp_angle);
    }



    // 2. Set motor speed directly based on newPosition
    motor_speed = map(newPosition, 0, 100, 0, 4095); // Adjust the max value (100 in this case) based on your rotary encoder
    motor_speed = constrain(motor_speed, 0, 4095);   // Ensure motor_speed is within 0-4095

    // 3. Determine direction based on encoder button press
    bool currentButtonState = digitalRead(ROTARY_ENCODER_C_PIN);
    if (currentButtonState == LOW && lastButtonState == HIGH) { // Assuming the button is active LOW
        currentDirection = !currentDirection;
        set_motor_direction(currentDirection);
    }
    lastButtonState = currentButtonState;


      // Calculate angle range (might correspond to something similar in the ESP code)
    if (angle_int > 0) {
        angle_range_min = -150 - angle_int;
        angle_range_max = 150 - angle_int;
    } else {
        angle_range_min = -150 + abs(angle_int);
        angle_range_max = 150 + abs(angle_int);
    }

    // 3. Print the Status
    String statusMessage = "ADC: " + String(potValue) + ", Rotary: " + String(newPosition);
    statusMessage += ", MotorSpeed: " + String(motor_speed) + ", Direction: " + (currentDirection ? "Forwards" : "Backwards");
    statusMessage += ", Angle: " + String(angle_int) + "." + String(abs(angle_int) % 100);
    statusMessage += ", Angle Range: [" + String(angle_range_min) + ", " + String(angle_range_max) + "]";
    Serial.println(statusMessage);

    // 4. Set Motor Speed using PWM
    analogWrite(MOTOR_IN1_PIN, motor_speed);
    analogWrite(MOTOR_IN2_PIN, 4095 - motor_speed);

    // 5. Introduce a Delay
    delay(10);
}

