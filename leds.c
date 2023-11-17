#include "leds.h"


void blink(int led_id)
{
    led_invert(led_id);
    nrf_delay_ms(SMALL_DELAY);
    led_invert(led_id);
    nrf_delay_ms(SMALL_DELAY); 
}

void blink_n_times(int led_id, int n)
{
    for (int j = 0; j < n; j++) {
        blink(led_id);
    }
    nrf_delay_ms(BIG_DELAY);
}

void led_init(int pin_n)
{
    nrf_gpio_cfg_output(pin_n);
    nrf_gpio_pin_write(pin_n, 1);
}

void led_invert(int pin_n)
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

void smooth_blink(int pin_n, int freq)
{
    nrfx_systick_init();

    uint32_t period = pow(10, 6) / freq; // T = (1 / V ) * 10^6 to make it in microseconds, 

    short percent = 0;

    while (percent < 100)
    {
        percent++;
        set_led_duty_cycle(percent, period, pin_n);
        nrf_delay_ms(2);
    }

    while (percent > 1)
    {
        percent --;
        set_led_duty_cycle(percent, period, pin_n);
        nrf_delay_ms(2);
    }
}

void set_led_duty_cycle(short percent, uint32_t period, int pin_n)
{
    if (percent < 1 || percent > 100) return;

    uint32_t time_on = period * percent / 100;
    uint32_t time_off = period - time_on;

    led_invert(pin_n);
    nrfx_systick_delay_us(time_on);
    led_invert(pin_n);
    nrfx_systick_delay_us(time_off);
}

void smooth_blink_init()
{
    led_init(LED1_PIN);
    led_init(LED2_R_PIN);
    led_init(LED2_G_PIN);
    led_init(LED2_B_PIN);
    nrfx_systick_init();
}