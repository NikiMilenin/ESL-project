#ifndef BUTTON_H
#define BUTTON_H


#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrfx_gpiote.h"
#include "app_timer.h"
#include "drv_rtc.h"

#define DEBOUNCING_DELAY 20
#define DOUBLE_CLICK_TIME 1000

#define SWITCH_BUTTON_PIN NRF_GPIO_PIN_MAP(1, 6)
void button_init(int);
bool button_pressed(int pin_n);
void double_click_init();
#endif