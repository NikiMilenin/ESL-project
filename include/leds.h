#ifndef LED_H
#define LED_H

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"
#include "app_usbd.h"
#include "app_usbd_serial_num.h"

#include <math.h>
#include <stdint.h>
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrfx_systick.h"
#include "nrfx_pwm.h"

#define SMALL_DELAY 400
#define BIG_DELAY 1000
#define VERY_BIG_DELAY 2000

#define LED1_PIN NRF_GPIO_PIN_MAP(0, 6)
#define LED2_R_PIN NRF_GPIO_PIN_MAP(0, 8)
#define LED2_G_PIN NRF_GPIO_PIN_MAP(1, 9)
#define LED2_B_PIN NRF_GPIO_PIN_MAP(0, 12)
#define PWM_STATE_TOP_VALUE 10000
#define PWM_RGB_TOP_VALUE 255

#define HSV_SCALER 100
#define HSV_IT 2

void led_init();
void check_state();
void correct_rgb();
void correct_hsv();

#endif