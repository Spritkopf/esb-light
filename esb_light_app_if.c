
#include <light/esb_light_app_if.h>
#include "led_effects.h"

#include "debug_swo.h"
void esb_light_app_set_state(uint8_t enabled)
{
    debug_swo_printf("Enable: %d", enabled);
}

void esb_light_app_set_brightness(uint8_t brightness)
{
    debug_swo_printf("Brightness: %d", brightness);
}

void esb_light_app_set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    debug_swo_printf("RGB: %d %d %d", r, g, b);
}

void esb_light_app_set_hsi(uint8_t h, uint8_t s, uint8_t i)
{
    debug_swo_printf("HSI: %d %d %d", h, s, i);
}

