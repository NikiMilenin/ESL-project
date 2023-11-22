#include "button.h"

volatile bool g_button_double_clicked;
volatile bool g_button_holded;
volatile int clicks;
APP_TIMER_DEF(debouncing_timer);
APP_TIMER_DEF(double_click_timer);
APP_TIMER_DEF(hold_button_timer);

void GPIOTEDoubleClick_EventHandler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    if (pin == SWITCH_BUTTON_PIN && action == NRF_GPIOTE_POLARITY_HITOLO) {
        app_timer_stop(debouncing_timer);
        app_timer_start(debouncing_timer, APP_TIMER_TICKS(DEBOUNCING_DELAY), 0);
    } 
}

void doubleClickTimer_EventHandler(void* ptr)
{
    clicks = 0;
}

void debouncingTimer_EventHandler(void* p)
{
    
    ++clicks;
    NRF_LOG_INFO("BUTTON PRESSED %d", clicks);
    if (clicks == 2)
    {
        NRF_LOG_INFO("BUTTON DOUBLE CKICLED");
        g_button_double_clicked = g_button_double_clicked == 1? 0 : 1;
        clicks = 0;
    }
    else  {
        app_timer_start(double_click_timer, APP_TIMER_TICKS(DOUBLE_CLICK_TIME), 0);
        app_timer_start(hold_button_timer, APP_TIMER_TICKS(HOLD_BUTTON_TIME), 0);
    }
}

void holdButtonTimer_EventHandler(void* ptr)
{
    if (nrf_gpio_pin_read(SWITCH_BUTTON_PIN) == 0) {
        NRF_LOG_INFO("BUTTON HOLDED");
        clicks = 0;
        g_button_holded = g_button_holded == 1? 0 : 1;
    }
}

void button_init()
{
    //nrf_gpio_cfg_input(SWITCH_BUTTON_PIN, NRF_GPIO_PIN_PULLUP);
    nrfx_gpiote_init();
    nrfx_gpiote_in_config_t config = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(1);
    config.pull = NRF_GPIO_PIN_PULLUP;
    nrfx_gpiote_in_init(SWITCH_BUTTON_PIN, &config, GPIOTEDoubleClick_EventHandler);
    nrfx_gpiote_in_event_enable(SWITCH_BUTTON_PIN, true);

    app_timer_init();
    app_timer_create(&debouncing_timer, APP_TIMER_MODE_SINGLE_SHOT, debouncingTimer_EventHandler);
    app_timer_create(&double_click_timer, APP_TIMER_MODE_SINGLE_SHOT, doubleClickTimer_EventHandler);
    app_timer_create(&hold_button_timer, APP_TIMER_MODE_SINGLE_SHOT, holdButtonTimer_EventHandler);
}