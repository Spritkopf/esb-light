#include "led_effects_static.h"
#include "pixel.h"

static volatile uint8_t g_update_flag = 0;

void led_effects_static_set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    for(int8_t i=0; i < PIXEL_NUM; i++){
        pixel_set_rgb(i, r, g, b);
    }

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

