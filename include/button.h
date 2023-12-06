#ifndef BUTTON_H
#define BUTTON_H

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"
#include "app_usbd.h"
#include "app_usbd_serial_num.h"

#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrfx_gpiote.h"
#include "app_timer.h"
#include "drv_rtc.h"

#define DEBOUNCING_DELAY 200
#define HOLD_BUTTON_TIME 500
#define DOUBLE_CLICK_TIME 1000

#define SWITCH_BUTTON_PIN NRF_GPIO_PIN_MAP(1, 6)
void button_init();
#endif