#ifndef LED_EFFECTS_FADE_H_
#define LED_EFFECTS_FADE_H_

#include <stdint.h>
#include "pixel.h"
typedef enum{
    LED_EFFECTS_FADE_ACTIVE = 0x00,
    LED_EFFECTS_FADE_DONE   = 0x01,
    LED_EFFECTS_FADE_ERROR  = 0x02
} led_effects_fade_status_t;


/*! \brief Fade pixel from currently active color to target color
 *  \param target_color Target Color
 *  \param time_msec duration of the fading in milliseconds
 *  \param mode fading mode (oneshot / continuously, see ::pixel_fade_mode_t)
 *  \returns 0 if OK, -1 on illegal pixel id or fading mode
 */
int8_t led_effects_fade_to_color(uint8_t id, color_t target_color, uint32_t time_msec, pixel_fade_mode_t mode);


/* \brief Execute fading
 * \returns LED_EFFECTS_FADE_ACTIVE if currently fading, 
 *          LED_EFFECTS_FADE_DONE if target RGB is reached, 
 *          LED_EFFECTS_FADE_ERROR on error 
 */
led_effects_fade_status_t led_effects_fade_execute(void);

#endif  // LED_EFFECTS_FADE_H_