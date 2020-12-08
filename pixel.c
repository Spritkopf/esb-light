
#include "pixel.h"
#include "colorwheel.h"
#include "nrfx_spi.h"
#include "nrf_gpio.h"

#define M_CHECK_PIXEL_ID(id)    do{if(id>PIXEL_NUM)return(-1);}while(0);
#define PIXEL_BUF_IDX_R     0
#define PIXEL_BUF_IDX_G     1
#define PIXEL_BUF_IDX_B     2

#define PIXEL_RGB_VAL_NUM  (PIXEL_NUM*3)

static nrfx_spi_t m_spi0 = NRFX_SPI_INSTANCE(0);
static void spi_init(void);
static void spi_xfer(uint8_t *data, uint8_t len);

static uint8_t g_pixel_rgb_buffer[PIXEL_RGB_VAL_NUM] = {0};

pixel_t pixels[PIXEL_NUM] = {0};


void pixel_init(void)
{
    /* init SPI interface */
    spi_init();

    /* set buffer pointers for pixels */
    for(uint8_t i = 0; i < PIXEL_RGB_VAL_NUM; i+=3){
        pixels[i].p_buf = &(g_pixel_rgb_buffer[i]);
    }

    /* clear all pixels */
    pixel_update();
}

int8_t pixel_get_rbg(uint8_t id, uint8_t *r, uint8_t *g, uint8_t *b)
{
    M_CHECK_PIXEL_ID(id);

    *r = pixels[id].p_buf[PIXEL_BUF_IDX_R];
    *g = pixels[id].p_buf[PIXEL_BUF_IDX_G];
    *b = pixels[id].p_buf[PIXEL_BUF_IDX_B];

    return (0);
}

int8_t pixel_set_rbg(uint8_t id, uint8_t r, uint8_t g, uint8_t b)
{
    M_CHECK_PIXEL_ID(id);

    pixels[id].p_buf[PIXEL_BUF_IDX_R] = r;
    pixels[id].p_buf[PIXEL_BUF_IDX_G] = g;
    pixels[id].p_buf[PIXEL_BUF_IDX_B] = b;

    return (0);
}

int8_t pixel_set_hsi(uint8_t id, uint16_t hue)
{
    M_CHECK_PIXEL_ID(id);

    colorwheel_get_rgb(hue, &(pixels[id].p_buf[PIXEL_BUF_IDX_R]), 
                            &(pixels[id].p_buf[PIXEL_BUF_IDX_G]), 
                            &(pixels[id].p_buf[PIXEL_BUF_IDX_B]));

    return (0);
}

int8_t pixel_get_hsi(uint8_t id, uint16_t *hue, uint8_t *intensity)
{
    M_CHECK_PIXEL_ID(id);

    *hue =  pixels[id].hue;
    *intensity =  pixels[id].intensity;

    return (0);
}

int8_t pixel_dim(uint8_t id, uint8_t intensity)
{
    M_CHECK_PIXEL_ID(id);

    colorwheel_set_brightness(intensity, &(pixels[id].p_buf[PIXEL_BUF_IDX_R]), 
                                         &(pixels[id].p_buf[PIXEL_BUF_IDX_G]), 
                                         &(pixels[id].p_buf[PIXEL_BUF_IDX_B]));

    return (0);
}

void pixel_update(void)
{
    spi_xfer((uint8_t*)g_pixel_rgb_buffer, PIXEL_RGB_VAL_NUM);
}

/********************** STATIC FUNCTIONS ***********************/

void spi_init(void)
{
    nrfx_spi_config_t config = NRFX_SPI_DEFAULT_CONFIG;
    config.sck_pin = NRF_GPIO_PIN_MAP(1,15);
    config.mosi_pin = NRF_GPIO_PIN_MAP(1,13);
    nrfx_spi_init(&m_spi0, &config, NULL, NULL);

}

static void spi_xfer(uint8_t *data, uint8_t len)
{
    nrfx_spi_xfer_desc_t xfer_desc = NRFX_SPI_XFER_TX(data, len);
    nrfx_spi_xfer(&m_spi0, &xfer_desc, 0);
}