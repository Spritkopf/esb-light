
#include "led_effects.h"
#include "nrfx_timer.h"

#include "debug_swo.h"
#include "nrf_gpio.h"

#define EFFECTS_UPDATE_FREQUENZY_HZ   1

const nrfx_timer_t g_effects_timer = NRFX_TIMER_INSTANCE(3);

static void effects_update_handler(nrf_timer_event_t event_type, void* p_context)
{
    if(event_type == NRF_TIMER_EVENT_COMPARE0){
        nrfx_timer_clear(&g_effects_timer);
        nrf_gpio_pin_toggle(NRF_GPIO_PIN_MAP(1, 12));
    }
}

/* start a timer for periodic updates of the enabled effects */
static void start_effects_timer(void)
{
    uint32_t timeout_ticks = nrfx_timer_ms_to_ticks(&g_effects_timer, 1000);
    nrfx_timer_compare(&g_effects_timer, NRF_TIMER_CC_CHANNEL0, timeout_ticks, true);
    nrfx_timer_enable(&g_effects_timer);
}

static void stop_effects_timer(void)
{
    nrfx_timer_clear(&g_effects_timer);
    nrfx_timer_disable(&g_effects_timer);
}

void led_effects_init(void)
{
    nrfx_timer_config_t timer_cfg = NRFX_TIMER_DEFAULT_CONFIG;
    timer_cfg.frequency = NRF_TIMER_FREQ_1MHz;
    timer_cfg.mode = NRF_TIMER_MODE_TIMER;
    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32;
    nrfx_timer_init(&g_effects_timer, &timer_cfg, effects_update_handler);
}

void led_effects_static_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    led_set_all(r, g, b);
    led_update();
    stop_effects_timer();

}

void led_effects_fade_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    start_effects_timer();
}

void led_effects_disable(void)
{
    led_effects_static_rgb(0, 0, 0);
}
