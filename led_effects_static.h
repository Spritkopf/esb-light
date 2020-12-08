#ifndef LED_EFFECTS_STATIC_H_
#define LED_EFFECTS_STATIC_H_

#include <stdint.h>


/*!
 * \brief Set static RGB 
 * \param r   value RED
 * \param g   value GREEN
 * \param b   value BLUE
 */
void led_effects_static_set_rgb(uint8_t r, uint8_t g, uint8_t b);

/*!
 * \brief Set static HSI 
 * \param hue       hue  (0 - 360)
 * \param intensity intensity  (0 - 255)
 * \retval 0    OK
 * \retval -1   Invalid hue (max: PIXEL_HUE_MAX)
 */
int8_t led_effects_static_set_hsi(uint16_t hue, uint8_t intensity);

/*!
 * \brief Execute the static effect
 * \details This function only updates the pixels if the static effect has been changed, not every time
 * \retval 0 OK     - changes applied successfully
 * \retval 1 Idle   - no changes were made
 */
int8_t led_effects_static_execute(void);

#endif  // LED_EFFECTS_STATIC_H_