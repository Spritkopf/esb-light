#ifndef LED_EFFECTS_H_
#define LED_EFFECTS_H_

#include <stdint.h>
#include "led.h"

typedef enum{
    LED_MODE_STATIC,
    LED_MODE_SPARKLE,
    LED_MODE_FADE,
} led_mode_t;

/* \brief Initialize effects module */
void led_effects_init(void);

/* \brief Set static RGB */
void led_effects_static_rgb(uint8_t r, uint8_t g, uint8_t b);

/* \brief Slowly Fade all LEDs
 */
void led_effects_fade_rgb(uint8_t r, uint8_t g, uint8_t b);

/* \brief disable all effects (turn LEDs off) */
void led_effects_disable(void);

#endif  // LED_EFFECTS_H_