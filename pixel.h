#ifndef PIXEL_H_
#define PIXEL_H_

#include <stdint.h>

#define PIXEL_NUM   50
#define PIXEL_HUE_MAX 360
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
 * \param intensity intensity value (0 - 255)
 * \returns 0 if OK, -1 on illegal pixel id
 */
int8_t pixel_dim(uint8_t id, uint8_t intensity);


/*! \brief Set color on all pixels
*/
void pixel_update(void);

#endif  // PIXEL_H_