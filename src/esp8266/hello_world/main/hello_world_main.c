#include "driver/adc.h"
#include "driver/pwm.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

// Define the GPIO pins for the L298N IN1 and IN2
#define MOTOR_IN1_PIN 5 // D1 on the ESP8266
#define MOTOR_IN2_PIN 4 // D2 on the ESP8266
#define PWM_PERIOD 1000 // 1kHz frequency

void app_main() {
  // Initialize ADC
  adc_config_t adc_config;
  adc_config.mode = ADC_READ_TOUT_MODE; // ADC TOUT pin mode
  adc_config.clk_div = 8; // ADC sample collection clock = 80MHz/clk_div = 10MHz
  adc_init(&adc_config);
  uint16_t potValue = 0; // This is where the ADC reading will be stored.

  // PWM initialization
  uint32_t pins[] = {MOTOR_IN1_PIN, MOTOR_IN2_PIN};
  uint32_t duties[] = {0, 0};
  int16_t phase[] = {0, 0};
  pwm_init(PWM_PERIOD, duties, 2, pins); // 2 channels for 2 pins
  pwm_set_phases(phase);
  pwm_start();

  /* Print chip information */
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  printf("This is ESP8266 chip with %d CPU cores, WiFi, ", chip_info.cores);
  printf("silicon revision %d, ", chip_info.revision);

  while (1) {
    esp_err_t ret = adc_read(&potValue); // Reading ADC value into potValue

    if (ret == ESP_OK) { // Checking if ADC read was successful
      printf("ADC Value: %d\n", potValue); // Printing the ADC value

      uint32_t motor_speed =
          (PWM_PERIOD * potValue) /
          1023; // Map ADC value (0-1023) to PWM duty (0-PWM_PERIOD)
      pwm_set_duty(0,
                   motor_speed); // Setting duty for channel 0 (MOTOR_IN1_PIN)
      pwm_set_duty(1,
                   PWM_PERIOD -
                       motor_speed); // Inverse for channel 1 (MOTOR_IN2_PIN),
                                     // so motor goes in one direction
      pwm_start();                   // Apply the changes
    } else {
      printf("Error reading ADC: %d\n", ret); // Print error code
    }
    vTaskDelay(100 / portTICK_PERIOD_MS); // Delay for 100ms
  }
}
