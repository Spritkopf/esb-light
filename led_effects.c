
#include "nrfx_timer.h"
#include "nrf_gpio.h"
#include "debug_swo.h"

#include "led_effects.h"
#include "led_effects_static.h"
#include "led_effects_fade.h"



typedef struct{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t percent; /* 0 - 100 */
    uint8_t speed;
} effect_state_fade_t;


const nrfx_timer_t g_effects_timer = NRFX_TIMER_INSTANCE(3);

static led_mode_t g_active_mode = LED_MODE_DISABLE;

static effect_state_fade_t g_effect_state_fade;

/* start a timer for periodic updates of the enabled effects */
static void start_effects_timer(void)
{
    uint32_t timeout_ticks = nrfx_timer_ms_to_ticks(&g_effects_timer, 1000/EFFECTS_UPDATE_FREQUENCY_HZ);
    nrfx_timer_compare(&g_effects_timer, NRF_TIMER_CC_CHANNEL0, timeout_ticks, true);
    nrfx_timer_enable(&g_effects_timer);
}

static void stop_effects_timer(void)
{
    nrfx_timer_clear(&g_effects_timer);
    nrfx_timer_disable(&g_effects_timer);
}

static void effects_update_handler(nrf_timer_event_t event_type, void* p_context)
{
    if(event_type == NRF_TIMER_EVENT_COMPARE0){
        nrfx_timer_clear(&g_effects_timer);
        
        switch(g_active_mode){
        case LED_MODE_STATIC:
            led_effects_static_execute();
            stop_effects_timer();
            g_active_mode = LED_MODE_DISABLE;
            break;
        case LED_MODE_FADE:
            if(led_effects_fade_execute() != LED_EFFECTS_FADE_ACTIVE){
                stop_effects_timer();
                g_active_mode = LED_MODE_DISABLE;
            }
            break;
        default:
            break;
        }
    }
}

void led_effects_init(void)
{
    nrfx_timer_config_t timer_cfg = NRFX_TIMER_DEFAULT_CONFIG;
    timer_cfg.frequency = NRF_TIMER_FREQ_1MHz;
    timer_cfg.mode = NRF_TIMER_MODE_TIMER;
    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32;
    nrfx_timer_init(&g_effects_timer, &timer_cfg, effects_update_handler);

    g_active_mode = LED_MODE_DISABLE;
}

void led_effects_set_mode(led_mode_t mode)
{
    g_active_mode = mode;
    start_effects_timer();
}

void led_effects_fade_rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t speed)
{
    g_active_mode = LED_MODE_FADE;
    g_effect_state_fade.r = r;
    g_effect_state_fade.g = g;
    g_effect_state_fade.b = b;
    g_effect_state_fade.speed = speed;

    start_effects_timer();
}

void led_effects_disable(void)
{
    color_t color = {0};
    led_effects_static_set_rgb(color);
}

//static void led_effect_fade_update
