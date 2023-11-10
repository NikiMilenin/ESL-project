#ifndef BUTTON_H
#define BUTTON_H

#include "nrf_gpio.h"
#include "nrf_delay.h"

#define SWITCH_BUTTON_PIN NRF_GPIO_PIN_MAP(1, 6)

void button_init(int);
bool button_pressed(int pin_n);

#endif