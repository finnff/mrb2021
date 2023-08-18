#include "driver/adc.h"
#include "driver/gpio.h"
#include "driver/pwm.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

// Define the GPIO pins for the L298N IN1 and IN2 for PWM
#define MOTOR_IN1_PIN 5 // D1 on the ESP8266
#define MOTOR_IN2_PIN 4 // D2 on the ESP8266

// Define the GPIO pins for the L298N IN1 and IN2 for Direction
#define MOTOR_DIRECTION_IN1 0 // D3 on the ESP8266
#define MOTOR_DIRECTION_IN2 2 // D4 on the ESP8266

#define ROTARY_ENCODER_A_PIN 14 // D5 on the ESP8266
#define ROTARY_ENCODER_B_PIN 12 // D6 on the ESP8266

#define PWM_PERIOD 1000 // 1kHz frequency
//
#define ADC_MIN 0
#define ADC_MAX 1024
#define TOTAL_ANGLE_RANGE 300

// Function to set motor direction
void set_motor_direction(bool forward)
{
    if (forward) {
        gpio_set_level(MOTOR_DIRECTION_IN1, 1);
        gpio_set_level(MOTOR_DIRECTION_IN2, 0);
    } else {
        gpio_set_level(MOTOR_DIRECTION_IN1, 0);
        gpio_set_level(MOTOR_DIRECTION_IN2, 1);
    }
}

void app_main()
{
    // Initialize ADC
    adc_config_t adc_config;
    adc_config.mode = ADC_READ_TOUT_MODE;
    adc_config.clk_div = 8;
    adc_init(&adc_config);
    uint16_t potValue = 0;

    // PWM initialization
    uint32_t pins[] = { MOTOR_IN1_PIN, MOTOR_IN2_PIN };
    uint32_t duties[] = { 0, 0 };
    float phase[] = { 0, 0 };
    pwm_init(PWM_PERIOD, duties, 2, pins);
    pwm_set_phases(phase);
    pwm_start();

    // Initialize the GPIO for motor direction
    gpio_set_direction(MOTOR_DIRECTION_IN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR_DIRECTION_IN2, GPIO_MODE_OUTPUT);

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP8266 chip with %d CPU cores, WiFi, ", chip_info.cores);
    printf("silicon revision %d, ", chip_info.revision);
    char* direction = "unknown";

    // Variables for calibration for pendulum Neutral (assumed at reset)
    uint16_t ADC_EQUILLIBRIUM = 0;
    int ANGLE_MIN_INT = 0;
    int ANGLE_MAX_INT = 0;
    bool calibrated = false;

    // Calibration
    printf("Calibrating Angle, Equilibrium and Range based off Neutral "
           "pendulum.\n");

    esp_err_t initial_ret = adc_read(&ADC_EQUILLIBRIUM);
    if (initial_ret == ESP_OK) {
        // Calculate available ADC range towards maximum from the equilibrium
        int adc_range_max = ADC_MAX - ADC_EQUILLIBRIUM;

        // Calculate proportion of the maximum angle we can achieve based on available ADC range
        float proportion_max = (float)adc_range_max / (ADC_MAX / 2); // Using ADC_MAX/2 since it represents the full range of positive angle movement

        // Calculate ANGLE_MAX_INT based on proportion of maximum possible
        ANGLE_MAX_INT = (int)(TOTAL_ANGLE_RANGE / 2.0 * proportion_max);

        // Calculate ANGLE_MIN_INT based on the remaining angle from total
        ANGLE_MIN_INT = TOTAL_ANGLE_RANGE - ANGLE_MAX_INT;

        // Since it should be symmetric about zero and negative, invert it
        ANGLE_MIN_INT = -ANGLE_MIN_INT;

        calibrated = true;
        printf("Calibration complete. ADC_EQUILLIBRIUM: %d, ANGLE_MIN_INT: %d, ANGLE_MAX_INT: %d\n", ADC_EQUILLIBRIUM, ANGLE_MIN_INT, ANGLE_MAX_INT);
    }

    while (1) {
        esp_err_t ret = adc_read(&potValue); // Reading ADC value into potValue

        if (ret == ESP_OK && calibrated) {
            int angle_int;

            if (potValue <= ADC_EQUILLIBRIUM) {
                float temp_angle = ANGLE_MIN_INT + (float)potValue / ADC_EQUILLIBRIUM * (-ANGLE_MIN_INT);
                angle_int = (int)(temp_angle);
            } else {
                float temp_angle = ((float)(potValue - ADC_EQUILLIBRIUM) / (ADC_MAX - ADC_EQUILLIBRIUM)) * ANGLE_MAX_INT;
                angle_int = (int)(temp_angle);
            }

            uint32_t motor_speed = (PWM_PERIOD * potValue) / 1023;

            // Set direction based on ROTARY_ENCODER_A_PIN's state
            if (gpio_get_level(ROTARY_ENCODER_A_PIN) == 1) {
                direction = "Forwards";
                set_motor_direction(true); // Forwards
            } else {
                direction = "Backwards";
                set_motor_direction(false); // Backwards
            }

            int angle_range_min, angle_range_max;

            if (angle_int > 0) {
                angle_range_min = ANGLE_MIN_INT - angle_int;
                angle_range_max = ANGLE_MAX_INT - angle_int;
            } else {
                angle_range_min = ANGLE_MIN_INT + abs(angle_int);
                angle_range_max = ANGLE_MAX_INT + abs(angle_int);
            }

            printf("ADC Value: %d, MotorSpeed: %d, Direction: %s, Angle: %d.%02d,Angle "
                   "Range: [%d, %d]\n",
                potValue, motor_speed, direction,
                angle_int, abs(angle_int) % 100,
                angle_range_min,
                angle_range_max);

            pwm_set_duty(0, motor_speed);
            pwm_set_duty(1, PWM_PERIOD - motor_speed);
            pwm_start();
        } else {
            printf("Error reading ADC: %d\n", ret);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
