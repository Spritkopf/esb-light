
#include "led_effects.h"
#include "led_effects_fade.h"


#define M_MSEC_TO_STEPS(msec)   (msec / (1000/EFFECTS_UPDATE_FREQUENCY_HZ))

led_effects_fade_status_t g_fade_status = LED_EFFECTS_FADE_DONE;

/* \brief Set fade target rgb */
int8_t led_effects_fade_to_color(uint8_t id, color_t target_color, uint32_t time_msec, pixel_fade_mode_t mode)
{
    if(pixel_fading_setup(id, target_color, M_MSEC_TO_STEPS(time_msec), mode) != 0){
        return (-1);
    }
    led_effects_set_mode(LED_MODE_FADE);
    g_fade_status = LED_EFFECTS_FADE_ACTIVE;

    return (0);
}

/* \brief Execute fading
 * \returns LED_EFFECTS_FADE_ACTIVE if currently fading, 
 *          LED_EFFECTS_FADE_DONE if target RGB is reached, 
 *          LED_EFFECTS_FADE_ERROR on error 
 */
led_effects_fade_status_t led_effects_fade_execute(void)
{
    if(g_fade_status == LED_EFFECTS_FADE_ACTIVE){
        pixel_fade_state_t status = FADE_IDLE; 
        
        for(uint8_t i = 0; i< PIXEL_NUM; i++){
            if(pixel_fading_execute(i) == FADE_ACTIVE){
                status = FADE_ACTIVE;
            }
        }
        
        pixel_update();
        
        if(status == FADE_IDLE){
            /* fading on all pixels done */
            g_fade_status = LED_EFFECTS_FADE_DONE;
        }
    }

    return (g_fade_status);

}
