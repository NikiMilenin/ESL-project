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

extern volatile bool g_button_double_clicked;
extern volatile bool g_button_holded;

int main(void)
{
    ret_code_t ret = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(ret);
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    smooth_blink_init();
    button_init();

    int leds[] ={LED1_PIN, LED2_R_PIN, LED2_G_PIN, LED2_B_PIN};
    int blinky_times[] = {
                        DEVICE_ID / 1000, 
                        DEVICE_ID % 1000 / 100, 
                        DEVICE_ID % 100 / 10, 
                        DEVICE_ID % 10
                        }; 

    int led_it = 0;
    int blinky_it = 0;
    int curr_it = 0;
    short percent = 0;
    signed char it = 1;

    uint32_t period = pow(10, 6) / PWM_FREQUENCY;

    while (true)
    {
        while(g_button_double_clicked)
        {
            percent += it;
            set_led_duty_cycle(percent, period, leds[led_it]);
            nrf_delay_ms(1);

            if (percent == 100)
            {
                it = -1;
            }
            else if (percent == 0)
            {
                ++curr_it;
                nrf_delay_ms(SMALL_DELAY);
                it = 1;
                if (curr_it == blinky_times[blinky_it])
                {
                    nrf_delay_ms(BIG_DELAY);
                    ++blinky_it;
                    blinky_it %= LED_N;
                    curr_it = 0;
                    ++led_it;
                    led_it %= LED_N;
                }
            }
            LOG_BACKEND_USB_PROCESS();
            NRF_LOG_PROCESS();  
        }
        set_led_duty_cycle(percent, period, leds[led_it]);
        nrf_delay_ms(1);
        LOG_BACKEND_USB_PROCESS();
        NRF_LOG_PROCESS();
    }
}