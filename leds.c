#include "leds.h"

static nrfx_pwm_t pwm_drv = NRFX_PWM_INSTANCE(0);

static nrfx_pwm_config_t pwm_config = NRFX_PWM_DEFAULT_CONFIG;

static nrf_pwm_values_individual_t seq_values = {1000, 1000, 1000, 1000};

extern volatile bool g_button_double_clicked;

nrf_pwm_sequence_t const seq = 
{
    .values.p_individual = &seq_values,
    .length          = NRF_PWM_VALUES_LENGTH(seq_values),
    .repeats         = 0,
    .end_delay       = 0
}; 

int it = -100;
void PWM_EventHandler(nrfx_pwm_evt_type_t event){
    NRF_LOG_INFO("PWM");
    if (g_button_double_clicked)
    {
        g_button_double_clicked = 0;
        seq_values.channel_0 += it;
    }
    if (seq_values.channel_0 == 0) it = 100;
    if (seq_values.channel_0 == 1000) it = -100;
}

void smooth_bl()
{
    pwm_config.output_pins[0] = LED1_PIN;
    pwm_config.output_pins[1] = LED2_R_PIN;
    pwm_config.output_pins[2] = LED2_G_PIN;
    pwm_config.output_pins[3] = LED2_B_PIN;
    pwm_config.irq_priority = 1;
    pwm_config.load_mode = NRF_PWM_LOAD_INDIVIDUAL;
    pwm_config.base_clock = NRF_PWM_CLK_1MHz;
    nrfx_pwm_init(&pwm_drv, &pwm_config, PWM_EventHandler);
    nrfx_pwm_simple_playback(&pwm_drv, &seq, 1, NRFX_PWM_FLAG_STOP);
}