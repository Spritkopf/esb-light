
#include "nrfx_rng.h"
#include "led_effects.h"
#include "led_effects_static.h"
#include "pixel.h"

static volatile uint8_t g_update_flag = 0;

void led_effects_static_set_rgb(color_t color)
{
    for(int8_t i=0; i < PIXEL_NUM; i++){
        pixel_set_rgb(i, color.r, color.g, color.b);
    }

    led_effects_set_mode(LED_MODE_STATIC);
    g_update_flag = 1;

}

int8_t led_effects_static_set_hsi(uint16_t hue, uint8_t intensity)
{
    if(hue > PIXEL_HUE_MAX){
        return (-1);
    }

    for(int8_t i=0; i < PIXEL_NUM; i++){
        pixel_set_hsi(i, hue, intensity);
    }

    led_effects_set_mode(LED_MODE_STATIC);
    g_update_flag = 1;

    return (0);
}

int8_t led_effects_static_execute(void)
{
    if(g_update_flag == 1){
        g_update_flag = 0;
        pixel_update();
        return (0);
    }

    return (1);
}   

