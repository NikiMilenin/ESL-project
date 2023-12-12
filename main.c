#include <stdbool.h>
#include <stdint.h>

#include "nordic_common.h"
#include "boards.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"
#include "app_usbd.h"
#include "app_usbd_serial_num.h"

#include "boards.h"
#include "button.h"
#include "leds.h"

#define LED_N 4
#define PWM_FREQUENCY 1000

#define DEVICE_ID 6579

int main(void)
{
    ret_code_t ret = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(ret);
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    button_init();
    led_init();

    while (true)
    {
        __WFE();
        LOG_BACKEND_USB_PROCESS();
        NRF_LOG_PROCESS();
    }
}