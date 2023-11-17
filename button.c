#include "button.h"

volatile bool BUTTON_DOUBLE_CLICKED;
volatile int clicks;
APP_TIMER_DEF(debouncing_timer);
APP_TIMER_DEF(double_click_timer);

void GPIOTEDoubleClick_EventHandler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action )
{
    if (pin == SWITCH_BUTTON_PIN && action == NRF_GPIOTE_POLARITY_HITOLO )
    {
        NRF_LOG_INFO("Button pressed");
        app_timer_stop(debouncing_timer);
        app_timer_start(debouncing_timer, APP_TIMER_TICKS(20), 0);
        app_timer_start(double_click_timer, APP_TIMER_TICKS(500), 0);
    }
}

void doubleClickTimer_EventHandler(void* ptr)
{
    clicks = 0;
}

void debouncingTimer_EventHandler(void* p)
{
    NRF_LOG_INFO("Click");
    ++clicks;
    if (clicks == 2)
    {
        NRF_LOG_INFO("Double click");
        BUTTON_DOUBLE_CLICKED = BUTTON_DOUBLE_CLICKED == 1? 0 : 1;
        clicks = 0;
    }
}

void double_click_init()
{
    nrfx_gpiote_init();
    nrfx_gpiote_in_config_t config = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(1);
    config.pull = NRF_GPIO_PIN_PULLUP;
    nrfx_gpiote_in_init(SWITCH_BUTTON_PIN, &config, GPIOTEDoubleClick_EventHandler);
    nrfx_gpiote_in_event_enable(SWITCH_BUTTON_PIN, true);

    app_timer_init();
    app_timer_create(&debouncing_timer, APP_TIMER_MODE_SINGLE_SHOT, debouncingTimer_EventHandler);
    app_timer_create(&double_click_timer, APP_TIMER_MODE_SINGLE_SHOT, doubleClickTimer_EventHandler);
}

void single_click_button_init(int pin_n)
{
    nrf_gpio_cfg_input(pin_n, NRF_GPIO_PIN_PULLUP);
}

bool button_pressed(int pin_n)
{
    return nrf_gpio_pin_read(pin_n) == 0;
}