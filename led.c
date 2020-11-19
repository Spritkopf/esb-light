
#include "led.h"
#include "nrfx_spi.h"
#include "nrf_gpio.h"


#define LED_MAX_NUM 50
#define COEFF_R 0.5
#define COEFF_G 0.6
#define COEFF_B 1.0

static uint8_t led_buffer[LED_MAX_NUM*3] = {0};

static nrfx_spi_t m_spi0 = NRFX_SPI_INSTANCE(0);

static void spi_xfer(uint8_t *data, uint8_t len);

void led_init(void)
{
    nrfx_spi_config_t config = NRFX_SPI_DEFAULT_CONFIG;
    config.sck_pin = NRF_GPIO_PIN_MAP(1,15);
    config.mosi_pin = NRF_GPIO_PIN_MAP(1,13);
    nrfx_spi_init(&m_spi0, &config, NULL, NULL);

}


void led_set_pixel(uint8_t idx, uint8_t r, uint8_t g, uint8_t b)
{
    if(idx >= LED_MAX_NUM){
        return;
    }

    led_buffer[(idx*3) + 0] = (uint8_t)((float)r * COEFF_R);
    led_buffer[(idx*3) + 1] = (uint8_t)((float)g * COEFF_G);
    led_buffer[(idx*3) + 2] = (uint8_t)((float)b * COEFF_B);
}

void led_set_all(uint8_t r, uint8_t g, uint8_t b)
{
    for (uint8_t i = 0; i < LED_MAX_NUM; i++){
        led_buffer[(i*3) + 0] = (uint8_t)((float)r * COEFF_R);
        led_buffer[(i*3) + 1] = (uint8_t)((float)g * COEFF_G);
        led_buffer[(i*3) + 2] = (uint8_t)((float)b * COEFF_B);
    }
}

void led_update(void)
{
    spi_xfer(led_buffer, LED_MAX_NUM*3);
}


/********************** STATIC FUNCTIONS ***********************/

static void spi_xfer(uint8_t *data, uint8_t len)
{
    nrfx_spi_xfer_desc_t xfer_desc = NRFX_SPI_XFER_TX(data, len);
    nrfx_spi_xfer(&m_spi0, &xfer_desc, 0);
}