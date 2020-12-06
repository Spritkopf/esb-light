#ifndef COLORWHEEL_H_
#define COLORWHEEL_H_

#include <stdint.h>

/*!
 * \brief          Look up RGB values in LUT by given angle
 * \param hue      the angle of the colorwheel (0 - 360)
 * \param *buf_r   pointer to target buffer RED
 * \param *buf_g   pointer to target buffer GREEN
 * \param *buf_b   pointer to target buffer BLUE
 * \returns -1 if invalid hue, 0 if OK
*/
int8_t colorwheel_get_rgb(uint16_t hue, uint8_t *buf_r, uint8_t *buf_g, uint8_t *buf_b);

/*!
 * \brief              Set Brightness (relative)
 * \param brightess    brightness from 0-255
 * \param *buf_r       pointer to target buffer RED
 * \param *buf_g       pointer to target buffer GREEN
 * \param *buf_b       pointer to target buffer BLUE
*/
void colorwheel_set_brightness(uint8_t brightness, uint8_t* buf_r, uint8_t* buf_g, uint8_t* buf_b);

#endif /* COLORWHEEL_H_ */
