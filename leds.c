#include "leds.h"
#include "logic.h"

static uint32_t HSV[3];
int hsv_it = HSV_IT;

nrfx_pwm_t state_led = NRFX_PWM_INSTANCE(0);

nrfx_pwm_t rgb_led = NRFX_PWM_INSTANCE(1);

static const nrfx_pwm_config_t state_pwm_config = {
    .output_pins = {
        LED1_PIN,
        NRFX_PWM_PIN_NOT_USED,
        NRFX_PWM_PIN_NOT_USED,
        NRFX_PWM_PIN_NOT_USED
    },
    .irq_priority = 7,
    .base_clock = NRF_PWM_CLK_1MHz,
    .count_mode = NRF_PWM_MODE_UP,
    .top_value = PWM_STATE_TOP_VALUE,
    .load_mode = NRF_PWM_LOAD_INDIVIDUAL,
    .step_mode = NRF_PWM_STEP_AUTO
};

static const nrfx_pwm_config_t rgb_pwm_config = {
    .output_pins = {
        NRFX_PWM_PIN_NOT_USED,
        LED2_R_PIN,
        LED2_G_PIN,
        LED2_B_PIN  
    },
    .irq_priority = 7,
    .base_clock = NRF_PWM_CLK_1MHz,
    .count_mode = NRF_PWM_MODE_UP,
    .top_value = PWM_RGB_TOP_VALUE,
    .load_mode = NRF_PWM_LOAD_INDIVIDUAL,
    .step_mode = NRF_PWM_STEP_AUTO
};

nrf_pwm_values_individual_t seq_values = {
    .channel_0 = 0,
    .channel_1 = 0,
    .channel_2 = 0,
    .channel_3 = 0
};


HSV_PICKER_MODES mode = 0;


int it_arr[] = {0, 50, 500, 0};
int it;

void PWM_state_handler(nrfx_pwm_evt_type_t event_type)
{
    check_state();
    seq_values.channel_0 += it;
    if (seq_values.channel_0 == 0 || seq_values.channel_0 == PWM_STATE_TOP_VALUE) it *= -1;
}

void PWM_RGB_handler(nrfx_pwm_evt_type_t event_type)
{
    if (button_holded()) {
        correct_hsv();
        set_rgb();
    }
}

void led_init()
{
    const nrf_pwm_sequence_t sequence = {
        .values.p_individual = &seq_values,
        .length = NRF_PWM_VALUES_LENGTH(seq_values),
        .repeats = 0,
        .end_delay = 0
    };
    nrfx_pwm_init(&state_led, &state_pwm_config, PWM_state_handler);
    nrfx_pwm_init(&rgb_led, &rgb_pwm_config, PWM_RGB_handler);
    read_state(HSV);
    set_rgb();
    nrfx_pwm_simple_playback(&state_led, &sequence, 1, NRFX_PWM_FLAG_LOOP);
    nrfx_pwm_simple_playback(&rgb_led, &sequence, 1, NRFX_PWM_FLAG_LOOP);
}

void check_state()
{
    if (button_double_clicked()) {
        ++mode;
        mode %= 4;
        hsv_it = 1;
        if (mode == BRIGHTNESS_PICKER_MODE) {
            seq_values.channel_0 = PWM_STATE_TOP_VALUE;
        } else {
            seq_values.channel_0 = 0;
        }
        if (mode == STAY_MODE) {
            write_state(HSV);
            read_state(HSV);
        }
        it = it_arr[mode];
    }
}

void correct_hsv()
{
    
    if (mode == HUE_PICKER_MODE) {
        if (HSV[0] == 0 || HSV[0] == 360 * HSV_SCALER) hsv_it *= -1;
        HSV[0] += hsv_it;
    } else if (mode == SATURATION_PICKER_MODE) {
        if (HSV[1] == 0 || HSV[1] == 100 * HSV_SCALER) hsv_it *= -1;
        HSV[1] += hsv_it;
    } else if (mode == BRIGHTNESS_PICKER_MODE) {
        if (HSV[2] == 0 || HSV[2] == 100 * HSV_SCALER) hsv_it *= -1;
        HSV[2] += hsv_it;
    }
}

void set_rgb()
{
    int H  = HSV[0] / HSV_SCALER;
    int S = HSV[1] / HSV_SCALER;
    int V = HSV[2] / HSV_SCALER;

    int i = (H / 60) % 6;
    float Vm = (100 - S) * V / 100;
    float a = (V - Vm) * (H % 60) / 60;
    float Vi = Vm + a;
    float Vd = V - a;

    switch(i) {
    case 0:
        seq_values.channel_1 = V * 255 / 100 ;
        seq_values.channel_2 = Vi * 255 / 100;
        seq_values.channel_3 = Vm * 255 / 100;
        break;
    case 1:
        seq_values.channel_1 = Vd * 255 / 100;
        seq_values.channel_2 = V * 255 / 100;
        seq_values.channel_3 = Vm * 255 / 100;
        break;
    case 2:
        seq_values.channel_1 = Vm * 255 / 100;
        seq_values.channel_2 = V * 255 / 100;
        seq_values.channel_3 = Vi * 255 / 100;
        break;
    case 3:
        seq_values.channel_1 = Vm * 255 / 100;
        seq_values.channel_2 = Vd * 255 / 100;
        seq_values.channel_3 = V * 255 / 100;
        break;
    case 4:
        seq_values.channel_1 = Vi * 255 / 100;
        seq_values.channel_2 = Vm * 255 / 100;
        seq_values.channel_3 = V * 255 / 100;
        break;
    case 5:
    default:
        seq_values.channel_1 = V * 255 / 100;
        seq_values.channel_2 = Vm * 255 / 100;
        seq_values.channel_3 = Vd * 255 / 100;
        break;
    }
}