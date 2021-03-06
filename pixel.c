#include <string.h>
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
    struct {
        pixel_fade_state_t state;
        pixel_fade_mode_t mode;
        color_t start_rgb;
        color_t target_rgb;
        color_t target_tmp_rgb;
        uint32_t target_steps;
        uint32_t current_step;
    } fade_info;
    uint8_t brightness;
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
    memset(pixels,0, sizeof(pixels));
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
    pixels[id].fade_info.state = FADE_IDLE;

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

    pixels[id].fade_info.state = FADE_IDLE;

    return (0);
}


int8_t pixel_set_brightness(uint8_t id, uint8_t brightness)
{
    M_CHECK_PIXEL_ID(id);

    pixels[id].brightness = brightness;

    return (0);
}

void pixel_update(void)
{
    for(uint8_t i = 0; i < PIXEL_NUM; i++){
        g_pixel_rgb_buffer[(i*3)+0] = pixels[i].current_rgb.r;
        g_pixel_rgb_buffer[(i*3)+1] = pixels[i].current_rgb.g;
        g_pixel_rgb_buffer[(i*3)+2] = pixels[i].current_rgb.b;

        colorwheel_set_brightness(pixels[i].brightness, &(g_pixel_rgb_buffer[(i*3)+0]), &(g_pixel_rgb_buffer[(i*3)+1]), &(g_pixel_rgb_buffer[(i*3)+2]));
    }
    spi_xfer((uint8_t*)g_pixel_rgb_buffer, PIXEL_RGB_VAL_NUM);
    timebase_delay_ms(1);
}

int8_t pixel_fading_setup(uint8_t id, color_t *target_rgb, color_t *start_rgb, uint32_t steps, uint32_t start_step, pixel_fade_mode_t mode)
{
    M_CHECK_PIXEL_ID(id);

    if((steps == 0) || (start_step >= steps)){
        return (-2);
    }

    if((mode != PIXEL_FADE_MODE_ONESHOT) && (mode != PIXEL_FADE_MODE_CONT)){
        return (-3);
    }

    if(target_rgb == NULL){
        return (-4);
    }

    if(start_rgb == NULL){
        pixels[id].fade_info.target_rgb = *target_rgb;
        pixels[id].fade_info.target_tmp_rgb = pixels[id].current_rgb;
    }else{
        pixels[id].fade_info.target_rgb = *start_rgb;
        pixels[id].fade_info.target_tmp_rgb = *target_rgb;
    }
    pixels[id].fade_info.start_rgb = pixels[id].current_rgb;
    pixels[id].fade_info.target_steps = steps;
    pixels[id].fade_info.current_step = start_step;
    pixels[id].fade_info.state = FADE_ACTIVE;
    pixels[id].fade_info.mode = mode;

    return (0);
}

int8_t pixel_fading_reverse(uint8_t id)
{
    M_CHECK_PIXEL_ID(id);

    pixels[id].fade_info.target_tmp_rgb = pixels[id].fade_info.target_rgb;
    pixels[id].fade_info.target_rgb = pixels[id].fade_info.start_rgb;
    pixels[id].fade_info.start_rgb = pixels[id].fade_info.target_tmp_rgb;

    if(pixels[id].fade_info.target_steps >= pixels[id].fade_info.current_step){
        pixels[id].fade_info.current_step = pixels[id].fade_info.target_steps - pixels[id].fade_info.current_step;
    }else{
        pixels[id].fade_info.current_step = 0;
    }
    pixels[id].fade_info.state = FADE_ACTIVE;

    return (0);
}

pixel_fade_state_t pixel_fading_execute(uint8_t id)
{
    if(id > PIXEL_NUM){
        return (FADE_ERROR);
    }
    uint8_t fade_finished_r = 0;
    uint8_t fade_finished_g = 0;
    uint8_t fade_finished_b = 0;
    
    /* check if fading is enabled on pixel */
    if(pixels[id].fade_info.state == FADE_ACTIVE){
        uint32_t current_step = pixels[id].fade_info.current_step;
        int32_t steps = pixels[id].fade_info.target_steps;
        int16_t r_diff = pixels[id].fade_info.target_rgb.r - pixels[id].fade_info.start_rgb.r;
        int16_t g_diff = pixels[id].fade_info.target_rgb.g - pixels[id].fade_info.start_rgb.g;
        int16_t b_diff = pixels[id].fade_info.target_rgb.b - pixels[id].fade_info.start_rgb.b;

        /* calculate rgb values for current step*/
        if(pixels[id].current_rgb.r != pixels[id].fade_info.target_rgb.r){
            pixels[id].current_rgb.r = (uint8_t)((int32_t)pixels[id].fade_info.start_rgb.r +(((int32_t)current_step*r_diff)/steps));
        }else{
            fade_finished_r = 1;
        }
        if(pixels[id].current_rgb.g != pixels[id].fade_info.target_rgb.g){
            pixels[id].current_rgb.g = (uint8_t)((int32_t)pixels[id].fade_info.start_rgb.g +(((int32_t)current_step*g_diff)/steps));
        }else{
            fade_finished_g = 1;
        }
        if(pixels[id].current_rgb.b != pixels[id].fade_info.target_rgb.b){
            pixels[id].current_rgb.b = (uint8_t)((int32_t)pixels[id].fade_info.start_rgb.b +(((int32_t)current_step*b_diff)/steps));
        }else{
            fade_finished_b = 1;
        }
        pixels[id].fade_info.current_step++;

        if((fade_finished_r==1) && (fade_finished_g==1) && (fade_finished_b==1)){
            if(pixels[id].fade_info.mode == PIXEL_FADE_MODE_ONESHOT){
                pixels[id].fade_info.state = FADE_IDLE;
            }else{
                if(memcmp(&(pixels[id].fade_info.target_tmp_rgb),&(pixels[id].fade_info.start_rgb), sizeof(color_t)) != 0){
                    pixels[id].fade_info.start_rgb = pixels[id].fade_info.target_tmp_rgb;
                }
                pixel_fading_reverse(id);
            }
        }
    }
    return (pixels[id].fade_info.state);
}
/********************** STATIC FUNCTIONS ***********************/

void spi_init(void)
{
    nrfx_spi_config_t config = NRFX_SPI_DEFAULT_CONFIG;
    config.frequency = NRF_SPI_FREQ_2M;
    config.sck_pin = NRF_GPIO_PIN_MAP(1,15);
    config.mosi_pin = NRF_GPIO_PIN_MAP(1,13);
    nrfx_spi_init(&m_spi0, &config, NULL, NULL);

}

static void spi_xfer(uint8_t *data, uint8_t len)
{
    nrfx_spi_xfer_desc_t xfer_desc = NRFX_SPI_XFER_TX(data, len);
    nrfx_spi_xfer(&m_spi0, &xfer_desc, 0);
}