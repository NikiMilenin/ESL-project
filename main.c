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
#include "buttons.h"
#include "leds.h"

#include "nrfx_gpiote.h"

#define LED_N 4
#define PWM_FREQUENCY 1000

#define DEVICE_ID 6579
volatile bool switched;

void GPIOTE_EventHandler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action )
{
    if (pin == SWITCH_BUTTON_PIN && action == NRF_GPIOTE_POLARITY_HITOLO )
    {
        NRF_LOG_INFO("Button pressed");
        switched = switched == 0 ? 1 : 0;
    }
}

int main(void)
{
    ret_code_t ret = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(ret);

    NRF_LOG_DEFAULT_BACKENDS_INIT();

    led_init(LED1_PIN);
    led_init(LED2_R_PIN);
    led_init(LED2_G_PIN);
    led_init(LED2_B_PIN);

    button_init(SWITCH_BUTTON_PIN);

    nrfx_systick_init();
    
    nrfx_gpiote_init();
    nrfx_gpiote_in_config_t config = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(1);
    config.pull = NRF_GPIO_PIN_PULLUP;
    nrfx_gpiote_in_init(SWITCH_BUTTON_PIN, &config, GPIOTE_EventHandler);
    nrfx_gpiote_in_event_enable(SWITCH_BUTTON_PIN, true);

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
        while(switched)
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
                    if (blinky_it == LED_N - 1) 
                    {
                        nrf_delay_ms(VERY_BIG_DELAY);
                        ++led_it;
                        led_it %= LED_N;
                    }
                    else
                    {
                        nrf_delay_ms(BIG_DELAY);
                    }
                    ++blinky_it;
                    blinky_it %= LED_N;
                    curr_it = 0;
                }
            }  
        }
        set_led_duty_cycle(percent, period, leds[led_it]);
        nrf_delay_ms(1);
        LOG_BACKEND_USB_PROCESS();
        NRF_LOG_PROCESS();
    }
}