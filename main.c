#include <stdbool.h>
#include <stdint.h>
#include "boards.h"
#include "led.h"
#include "button.h"

#define LED_N 4
#define PWM_FREQUENCY 1000

#define DEVICE_ID 6579

void button_hold_smooth_blinky();

int main(void)
{
    led_init(LED1_PIN);
    led_init(LED2_R_PIN);
    led_init(LED2_G_PIN);
    led_init(LED2_B_PIN);

    button_init(SWITCH_BUTTON_PIN);

    nrfx_systick_init();
    button_hold_smooth_blinky();
}

void button_hold_smooth_blinky()
{

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
        while(button_pressed(SWITCH_BUTTON_PIN))
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
    }
}