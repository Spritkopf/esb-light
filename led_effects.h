#ifndef LED_EFFECTS_H_
#define LED_EFFECTS_H_

#include <stdint.h>

#define EFFECTS_UPDATE_FREQUENCY_HZ   30

typedef enum{
    LED_MODE_STATIC,
    LED_MODE_SPARKLE,
    LED_MODE_FADE,
    LED_MODE_DISABLE,
} led_mode_t;

/* \brief Initialize effects module */
void led_effects_init(void);

void led_effects_set_mode(led_mode_t mode);

/* \brief disable all effects (turn LEDs off) */
void led_effects_disable(void);

#endif  // LED_EFFECTS_H_