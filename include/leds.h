#ifndef LED_H
#define LED_H

#include <math.h>
#include <stdint.h>
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrfx_systick.h"

#define SMALL_DELAY 400
#define BIG_DELAY 1000
#define VERY_BIG_DELAY 2000

#define LED1_PIN NRF_GPIO_PIN_MAP(0, 6)
#define LED2_R_PIN NRF_GPIO_PIN_MAP(0, 8)
#define LED2_G_PIN NRF_GPIO_PIN_MAP(1, 9)
#define LED2_B_PIN NRF_GPIO_PIN_MAP(0, 12)
#define DEFAULT_LED_PWM_FREQUENCY 1000

void blink(int);
void blink_n_times(int, int);
void led_init(int);
void led_invert(int);
void smooth_blink(int, int);
void set_led_duty_cycle(short, uint32_t, int);

#endif