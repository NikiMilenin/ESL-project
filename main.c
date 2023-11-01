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

#define LED_N 4

void blink(int);
void blink_n_times(int, int);
void my_led_invert(int);


/**
 * @brief Function for application main entry.
 */
int main(void)
{
    /* Configure board. */
    bsp_board_init(BSP_INIT_LEDS);
    int leds[] ={LED1_PIN, LED2_R_PIN, LED2_G_PIN, LED2_B_PIN};

    /* Toggle LEDs. */
    while (true)
    {
        for (int i = 0; i < LED_N; i++)
        {
            int led_id = leds[i];

            blink_n_times(led_id, 6);
            blink_n_times(led_id, 5);
            blink_n_times(led_id, 7);
            blink_n_times(led_id, 9);
            nrf_delay_ms(VERY_BIG_DELAY);
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

void my_led_invert(int pin_n)
{
    nrf_gpio_cfg_output(pin_n);
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