#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include <driver/gpio.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct _rotary_encoder rotary_encoder_t;

typedef void (*rotary_encoder_callback_fn)(uint8_t position, void *context);

rotary_encoder_t *rotary_encoder_create(uint8_t gpio_a, uint8_t gpio_b,
                                        rotary_encoder_callback_fn callback,
                                        void *context);
void rotary_encoder_delete(rotary_encoder_t *rotary_encoder);

uint8_t rotary_encoder_get_position(rotary_encoder_t *rotary_encoder);
void rotary_encoder_set_position(rotary_encoder_t *rotary_encoder,
                                 uint8_t position);

struct _rotary_encoder {
  uint8_t gpio_a;
  uint8_t gpio_b;
  rotary_encoder_callback_fn callback;
  void *context;

  uint8_t last_state;
  uint8_t position;

  struct _rotary_encoder *next;
};

// Internal Functions
static void rotary_encoder_process(rotary_encoder_t *rotary_encoder);
static void rotary_encoder_timer_callback(TimerHandle_t timer);
static rotary_encoder_t *rotary_encoder_find_by_gpio(uint8_t gpio_a,
                                                     uint8_t gpio_b);
static int rotary_encoders_init();

// Global Variables
static SemaphoreHandle_t rotary_encoders_lock = NULL;
static rotary_encoder_t *rotary_encoders = NULL;
static TimerHandle_t rotary_encoder_timer = NULL;
static bool rotary_encoders_initialized = false;

// ESP8266 GPIO functions instead of my_gpio_* functions
static inline uint8_t rotary_encoder_gpio_read(uint8_t gpio) {
  return gpio_read(gpio);
}

static inline void rotary_encoder_gpio_enable(uint8_t gpio) {
  gpio_enable(gpio, GPIO_INPUT);
}

static inline void rotary_encoder_gpio_pullup(uint8_t gpio) {
  gpio_set_pullup(gpio, true, true);
}
