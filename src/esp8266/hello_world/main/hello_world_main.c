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
void set_motor_direction(bool forward) {
  if (forward) {
    gpio_set_level(MOTOR_DIRECTION_IN1, 1);
    gpio_set_level(MOTOR_DIRECTION_IN2, 0);
  } else {
    gpio_set_level(MOTOR_DIRECTION_IN1, 0);
    gpio_set_level(MOTOR_DIRECTION_IN2, 1);
  }
}

void app_main() {
  // Initialize ADC
  adc_config_t adc_config;
  adc_config.mode = ADC_READ_TOUT_MODE;
  adc_config.clk_div = 8;
  adc_init(&adc_config);
  uint16_t potValue = 0;

  // PWM initialization
  uint32_t pins[] = {MOTOR_IN1_PIN, MOTOR_IN2_PIN};
  uint32_t duties[] = {0, 0};
  float phase[] = {0, 0};
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
  char *direction = "unknown";

  // Variables for calibration for pendulum Neutral (assumed at reset)
  uint16_t ADC_EQUILLIBRIUM = 0;
  float ANGLE_MIN = 0;
  float ANGLE_MAX = 0;
  bool calibrated = false;

  // Calibration
  printf("Calibrating Angle, Equilibrium and Range based off Neutral pendulum."
}
esp_err_t initial_ret = adc_read(&ADC_EQUILLIBRIUM);
if (initial_ret == ESP_OK) {
  ANGLE_MAX = TOTAL_ANGLE_RANGE / 2.0;
  ANGLE_MIN = -ANGLE_MAX;
  calibrated = true;
}

while (1) {
  esp_err_t ret = adc_read(&potValue); // Reading ADC value into potValue

  if (ret == ESP_OK && calibrated) {

    float angle;
    if (potValue <= ADC_EQUILLIBRIUM) {
      angle = ANGLE_MIN + (float)potValue / ADC_EQUILLIBRIUM * (-ANGLE_MIN);
    } else {
      angle = ((float)(potValue - ADC_EQUILLIBRIUM) /
               (ADC_MAX - ADC_EQUILLIBRIUM)) *
              ANGLE_MAX;
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

    printf("ADC Value: %d, MotorSpeed: %d, Direction: %s, Angle: %f, Angle "
           "Range: [%f, %f]\n",
           potValue, motor_speed, direction, angle, ANGLE_MIN, ANGLE_MAX);

    pwm_set_duty(0, motor_speed);
    pwm_set_duty(1, PWM_PERIOD - motor_speed);
    pwm_start();
  } else {
    printf("Error reading ADC: %d\n", ret);
  }
  vTaskDelay(100 / portTICK_PERIOD_MS);
}
}
