
#include "led_effects.h"


void led_effects_static_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    led_set_all(r, g, b);
    led_update();
}


void led_effects_disable(void)
{
    led_effects_static_rgb(0, 0, 0);
}
