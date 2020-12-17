
#include <string.h>
#include <stdint.h>
#include "esb_protocol.h"
#include "debug_swo.h"

#include "pixel.h"
#include "led_effects.h"
#include "led_effects_static.h"
#include "led_effects_fade.h"


#define ESB_CMD_STATIC_RGB       0x10    /* set static RGB value for all LEDs */
#define ESB_CMD_SET_RGB_SPARKLE  0x11    /* set RGB values sparkling effect */
#define ESB_CMD_FADE             0x12    /* set RGB values fading effect */
#define ESB_CMD_FADE_CONT        0x13    /* set RGB values and fade between current RGB and target RGB continuously */
#define ESB_CMD_DISABLE          0x20    /* Disable LEDs */



static uint8_t esb_listener_address[5] = {100,100,100,100,1};
static uint8_t rx_payload[32];
static uint8_t rx_payload_length;
static volatile uint8_t pending_flag = 0;

static void esb_listener_callback(uint8_t *payload, uint8_t payload_length)
{
    debug_swo_printf("received data: length: %d, cmd=%02X\n", payload_length, payload[0]);
    memcpy(rx_payload, payload, payload_length);
    rx_payload_length = payload_length;
    pending_flag = 1;
}

int8_t esb_protocol_init(void)
{
    int8_t err_code = esb_init();
    esb_set_pipeline_address(ESB_PIPE_1, esb_listener_address);
    esb_start_listening(ESB_PIPE_1, esb_listener_callback);

    return (err_code);
}

int8_t esb_protocol_process(void)
{
    if(pending_flag == 1)
    {
        pending_flag = 0;

        uint8_t cmd = rx_payload[0];
        switch(cmd){
        case ESB_CMD_STATIC_RGB:
            if (rx_payload_length == 4){
                color_t color = {.r = rx_payload[1], .g=rx_payload[2], .b=rx_payload[3]};
                led_effects_static_set_rgb(color);
            }
            break;
        case ESB_CMD_FADE:
            if (rx_payload_length == 9){
                // payload: [cmd, pixel_id, r, g, b, time]
                color_t target_color = {.r=rx_payload[2], .g=rx_payload[3], .b=rx_payload[4]};
                uint32_t time = *(uint32_t*)&(rx_payload[5]);
                led_effects_fade_to_color(rx_payload[1], target_color, time, PIXEL_FADE_MODE_ONESHOT);
            }
            break;
        case ESB_CMD_FADE_CONT:
            if (rx_payload_length == 9){
                // payload: [cmd, pixel_id, r, g, b, time]
                color_t target_color = {.r=rx_payload[2], .g=rx_payload[3], .b=rx_payload[4]};
                uint32_t time = *(uint32_t*)&(rx_payload[5]);
                led_effects_fade_to_color(rx_payload[1], target_color, time, PIXEL_FADE_MODE_CONT);
            }
            break;
        case ESB_CMD_DISABLE:
            led_effects_disable();
            break;
        default:
            break;
        }
    }
    return (0);
}