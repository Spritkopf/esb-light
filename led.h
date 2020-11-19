#ifndef LED_H__
#define LED_H__

#include <stdint.h>

/*! \brief Iniitalize LED module (SPI interface)
*/
void led_init(void);

/*! \brief Set RGB values for a specific pixel
 * \param idx   Index of the pixel (0 - 49)
 * \param r     Red value (0 - 255)
 * \param g     Green value (0 - 255)
 * \param b     Blue value (0 - 255)
 * 
 * \note This function only sets the internal buffer, 
 * use led_update() to write the colors to the LED strip
*/
void led_set_pixel(uint8_t idx, uint8_t r, uint8_t g, uint8_t b);

/*! \brief Set RGB values for all pixel
 * \param r     Red value (0 - 255)
 * \param g     Green value (0 - 255)
 * \param b     Blue value (0 - 255)
 * 
 * \note This function only sets the internal buffer, 
 * use led_update() to write the colors to the LED strip
*/
void led_set_all(uint8_t r, uint8_t g, uint8_t b);

/*! \brief Update LED strip with values from internal buffer
*/
void led_update(void);

#endif //LED_H__