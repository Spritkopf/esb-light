
#include "pixel.h"
#include "colorwheel.h"
#include "nrfx_spi.h"
#include "nrf_gpio.h"
#include "timebase.h"

#define M_CHECK_PIXEL_ID(id)    do{if(id>PIXEL_NUM)return(-1);}while(0);
#define PIXEL_BUF_IDX_R     0
#define PIXEL_BUF_IDX_G     1
#define PIXEL_BUF_IDX_B     2

#define PIXEL_RGB_VAL_NUM  (PIXEL_NUM*3)

typedef struct{
    color_t current_rgb;
} pixel_t;




static nrfx_spi_t m_spi0 = NRFX_SPI_INSTANCE(0);
static void spi_init(void);
static void spi_xfer(uint8_t *data, uint8_t len);

static uint8_t g_pixel_rgb_buffer[PIXEL_RGB_VAL_NUM] = {0};

pixel_t pixels[PIXEL_NUM] = {0};


void pixel_init(void)
{
    /* init SPI interface */
    spi_init();

    /* clear all pixels */
    pixel_update();
}

int8_t pixel_get_rgb(uint8_t id, uint8_t *r, uint8_t *g, uint8_t *b)
{
    M_CHECK_PIXEL_ID(id);

    *r = pixels[id].current_rgb.r;
    *g = pixels[id].current_rgb.g;
    *b = pixels[id].current_rgb.b;

    return (0);
}

int8_t pixel_set_rgb(uint8_t id, uint8_t r, uint8_t g, uint8_t b)
{
    M_CHECK_PIXEL_ID(id);

    pixels[id].current_rgb.r = r;
    pixels[id].current_rgb.g = g;
    pixels[id].current_rgb.b = b;

    return (0);
}

int8_t pixel_set_hsi(uint8_t id, uint16_t hue, uint8_t intensity)
{
    M_CHECK_PIXEL_ID(id);

    colorwheel_get_rgb(hue, &(pixels[id].current_rgb.r), 
                            &(pixels[id].current_rgb.g), 
                            &(pixels[id].current_rgb.b));

    colorwheel_set_brightness(intensity, &(pixels[id].current_rgb.r), 
                                         &(pixels[id].current_rgb.g), 
                                         &(pixels[id].current_rgb.b));

    return (0);
}


int8_t pixel_dim(uint8_t id, uint8_t intensity)
{
    M_CHECK_PIXEL_ID(id);

    colorwheel_set_brightness(intensity, &(pixels[id].current_rgb.r), 
                                         &(pixels[id].current_rgb.g), 
                                         &(pixels[id].current_rgb.b));

    return (0);
}

void pixel_update(void)
{
    for(uint8_t i = 0; i < PIXEL_NUM; i++){
        g_pixel_rgb_buffer[(i*3)+0] = pixels[i].current_rgb.r;
        g_pixel_rgb_buffer[(i*3)+1] = pixels[i].current_rgb.g;
        g_pixel_rgb_buffer[(i*3)+2] = pixels[i].current_rgb.b;
    }
    spi_xfer((uint8_t*)g_pixel_rgb_buffer, PIXEL_RGB_VAL_NUM);
    timebase_delay_ms(1);
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