#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"
#include "../esl-nsdk/modules/nrfx/hal/nrf_gpio.h"

#define SMALL_DELAY 400
#define BIG_DELAY 1000
#define VERY_BIG_DELAY 2000

#define LED1_PIN NRF_GPIO_PIN_MAP(0, 6)
#define LED2_R_PIN NRF_GPIO_PIN_MAP(0, 8)
#define LED2_G_PIN NRF_GPIO_PIN_MAP(1, 9)
#define LED2_B_PIN NRF_GPIO_PIN_MAP(0, 12)

#define SWITCH_BUTTON_PIN NRF_GPIO_PIN_MAP(1, 6)

#define LED_N 4

void blink(int);
void blink_n_times(int, int);
void my_led_init(int);
void my_led_invert(int);
void my_button_init(int);
bool button_pressed(int pin_n);

int main(void)
{

    my_led_init(LED1_PIN);
    my_led_init(LED2_R_PIN);
    my_led_init(LED2_G_PIN);
    my_led_init(LED2_B_PIN);

    my_button_init(SWITCH_BUTTON_PIN);

    int leds[] ={LED1_PIN, LED2_R_PIN, LED2_G_PIN, LED2_B_PIN}; // array of led pin's id
    int blinky_times[] = {6*2, 5*2, 7*2, 9*2}; // n * 2 to on and off led in each iteration

    int led_it = 0;
    int blinky_it = 0;
    int curr_it = 0;
    while (true)
    {
        while(button_pressed(SWITCH_BUTTON_PIN))
        {
            
            curr_it++;
            my_led_invert(leds[led_it]);
            nrf_delay_ms(SMALL_DELAY);

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
}

void blink(int led_id)
{
    my_led_invert(led_id);
    nrf_delay_ms(SMALL_DELAY);
    my_led_invert(led_id);
    nrf_delay_ms(SMALL_DELAY); 
}

void blink_n_times(int led_id, int n)
{
    for (int j = 0; j < n; j++) {
        blink(led_id);
    }
    nrf_delay_ms(BIG_DELAY);
}

void my_led_init(int pin_n)
{
    nrf_gpio_cfg_output(pin_n);
    nrf_gpio_pin_write(pin_n, 1);
}

void my_led_invert(int pin_n)
{
    int prev_state = nrf_gpio_pin_out_read(pin_n);
    if (prev_state == 0)
    {
        nrf_gpio_pin_write(pin_n, 1);
    }
    else
    {
        nrf_gpio_pin_write(pin_n, 0);
    }
}

void my_button_init(int pin_n)
{
    nrf_gpio_cfg_input(pin_n, NRF_GPIO_PIN_PULLUP);
}

bool button_pressed(int pin_n)
{
    return nrf_gpio_pin_read(pin_n) == 0;
}