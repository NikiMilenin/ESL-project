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

#define HOLD_BUTTON_DEB_TIME_US 200
#define DEBOUNCING_DELAY_US 75
#define HOLD_BUTTON_TIME_US 350
#define DOUBLE_CLICK_TIME_US 500

#define SWITCH_BUTTON_PIN NRF_GPIO_PIN_MAP(1, 6)
void button_init();
bool button_double_clicked();
bool button_holded();
#endif