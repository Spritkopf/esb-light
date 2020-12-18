#ifndef PIXEL_H_
#define PIXEL_H_

#include <stdint.h>

#define PIXEL_NUM   50
#define PIXEL_HUE_MAX 360
typedef enum {
    FADE_IDLE   = 0x00,
    FADE_ACTIVE = 0x01,
    FADE_ERROR = 0x02
} pixel_fade_state_t;

typedef enum{
    PIXEL_FADE_MODE_ONESHOT = 0x00,   /* only fade to target color and stop */
    PIXEL_FADE_MODE_CONT = 0x01       /* fade to target, then back to beginning, and so on */
} pixel_fade_mode_t;

typedef struct{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color_t;

/*!
 * \brief Initialize pixel module 
 */
void pixel_init(void);

/*!
 * \brief Get RGB Value of specific pixel
 * \param id Pixel number (maximum PIXEL_NUM)
 * \param r   pointer to buffer RED
 * \param g   pointer to buffer GREEN
 * \param b   pointer to buffer BLUE
 * \returns 0 if OK, -1 on illegal pixel id
 */
int8_t pixel_get_rgb(uint8_t id, uint8_t *r, uint8_t *g, uint8_t *b);

/*!
 * \brief Set RGB Value of specific pixel
 * \param id Pixel number (maximum PIXEL_NUM)
 * \param r   value RED
 * \param g   value GREEN
 * \param b   value BLUE
 * \returns 0 if OK, -1 on illegal pixel id
 */
int8_t pixel_set_rgb(uint8_t id, uint8_t r, uint8_t g, uint8_t b);



/*!
 * \brief Set HSI Value of specific pixel
 * \param id Pixel number (maximum PIXEL_NUM)
 * \param hue hue value (0 - 360)
 * \param intensity intensity value (0 - 255)
 * \returns 0 if OK, -1 on illegal pixel id
 * \note    Saturation is always 1.0
 */
int8_t pixel_set_hsi(uint8_t id, uint16_t hue, uint8_t intensity);

/*!
 * \brief Set Brightness of specific pixel
 * \param id Pixel number (maximum PIXEL_NUM)
 * \param intensity intensity value (0 - 255, 0 = off, 255 = full brightness)
 * \returns 0 if OK, -1 on illegal pixel id
 */
int8_t pixel_dim(uint8_t id, uint8_t intensity);

/*!
 * \brief Setup fading
 * \param id Pixel number (maximum PIXEL_NUM)
 * \param target_rgb pointer to target color (RGB values)
 * \param start_color pointer to start color (set to NULL to use current color)
 * \param steps amount of steps the fading should take (i.e. how many times "execute" must be called for the fade to complete)
 * \param mode fading mode (oneshot / continuously, see ::pixel_fade_mode_t)
 * \returns 0 if OK, -1 on illegal pixel id, -2 on step value, -3 illegal mode, -4 NULL pointer
 * \details If start color is supplied, pixel will fade from current to start color, then alternate between target and start color (only in continuous mode)
 * \note Set "steps" to 1 to change color immediately
 */
int8_t pixel_fading_setup(uint8_t id, color_t *target_rgb, color_t *start_rgb, uint32_t steps, pixel_fade_mode_t mode);

/*!
 * \brief Reverse fading
 * \param id Pixel number (maximum PIXEL_NUM)
 * \details This function switches start and target color of a pixels fading configuration.
 *          Fading must be setup before by calling ::pixel_fading_setup. If the fading status
 *          of the specified pixel is currently IDLE, fading is restarted 
 * \returns 0 if OK, -1 on illegal pixel id
 */
int8_t pixel_fading_reverse(uint8_t id);

/*!
 * \brief Execute fading steps on specific pixel
 * \param id Pixel number (maximum PIXEL_NUM)
 * \retval FADE_IDLE - done fading (current color = target color)
 * \retval FADE_ACTIVE - fading in progress
 * \retval FADE_ERROR - Illegal pixel ID provided
 */
pixel_fade_state_t pixel_fading_execute(uint8_t id);


/*! \brief Set color on all pixels
*/
void pixel_update(void);

#endif  // PIXEL_H_