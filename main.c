#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"

#define SMALL_DELAY 400
#define BIG_DELAY 1000
#define VERY_BIG_DELAY 2000

void blink(int);
void blink_n_times(int, int);


/**
 * @brief Function for application main entry.
 */
int main(void)
{
    /* Configure board. */
    bsp_board_init(BSP_INIT_LEDS);

    /* Toggle LEDs. */
    while (true)
    {
        for (int i = 0; i < LEDS_NUMBER; i++)
        {
            blink_n_times(i, 6);
            blink_n_times(i, 5);
            blink_n_times(i, 7);
            blink_n_times(i, 9);
            nrf_delay_ms(VERY_BIG_DELAY);
        }
    }
}

void blink(int led_id)
{
    bsp_board_led_invert(led_id);
    nrf_delay_ms(SMALL_DELAY);
    bsp_board_led_invert(led_id);
    nrf_delay_ms(SMALL_DELAY);
}

void blink_n_times(int led_id, int n)
{
    for (int j = 0; j < n; j++) {
        blink(led_id);
    }
    nrf_delay_ms(BIG_DELAY);
}