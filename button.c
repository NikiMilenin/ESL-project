#include "button.h"

void button_init(int pin_n)
{
    nrf_gpio_cfg_input(pin_n, NRF_GPIO_PIN_PULLUP);
}

bool button_pressed(int pin_n)
{
    return nrf_gpio_pin_read(pin_n) == 0;
}