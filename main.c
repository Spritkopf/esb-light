/**
 * Copyright (c) 2014 - 2020, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "sdk_common.h"
#include "nrf.h"
#include "nrf_esb.h"
#include "nrf_error.h"
#include "nrf_esb_error_codes.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "nrf_delay.h"
#include "app_util.h"
#include "nrfx_gpiote.h"
#include "nrf_drv_clock.h"

#include "timebase.h"
#include "esb.h"
#include "debug_swo.h"
#include "led.h"

static volatile uint32_t update_flag = 1;

static uint8_t esb_listener_address[5] = {100,100,100,100,1};

uint8_t test_colors[3] = {0};

static void esb_listener_callback(uint8_t *payload, uint8_t payload_length)
{
    debug_swo_printf("received data: length: %d, cmd=%02X\n", payload_length, payload[0]);
    switch(payload[0]){
    case 0x10:
        if (payload_length == 4){
            test_colors[0] = payload[1];
            test_colors[1] = payload[2];
            test_colors[2] = payload[3];
            update_flag = 1;
        }
        break;
    default:
        break;
    }
}

static void button_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action){
    static uint8_t color_index = 0;

    debug_swo_printf("BUTTON PRESSED\n");

    memset(test_colors, 0x00, 3);
    test_colors[color_index] = 0xFF;
    color_index++;
    if(color_index==3){
        color_index = 0;
    }
    
    update_flag = 1;
}


void clocks_start( void )
{
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART = 1;

    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);
}


void gpio_init( void )
{
    nrfx_gpiote_pin_t button_pin = NRF_GPIO_PIN_MAP(0, 11);
    nrfx_gpiote_in_config_t button_cfg = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(false);
    button_cfg.pull = NRF_GPIO_PIN_PULLUP;
    nrfx_gpiote_init();
    nrfx_gpiote_in_init(button_pin, &button_cfg, button_handler);
    nrfx_gpiote_in_event_enable(button_pin, true);

    /* LED */
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(1, 12));
    nrf_gpio_pin_clear(NRF_GPIO_PIN_MAP(1, 12));
}

void rf_antenna_init(void)
{
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 24));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 25));

    /* Enable PCB antenna */
    nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(0, 24));
    nrf_gpio_pin_clear(NRF_GPIO_PIN_MAP(0, 25));
}

int main(void)
{
    ret_code_t err_code;
    UNUSED_VARIABLE(err_code);

    debug_swo_init();

    gpio_init();

    debug_swo_printf("startup\n");
    clocks_start();

    timebase_init();
    
    /* turn user LED on*/
    nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(1, 12));

    rf_antenna_init();

    led_init();

    err_code = esb_init();
    esb_set_pipeline_address(ESB_PIPE_1, esb_listener_address);
    esb_start_listening(ESB_PIPE_1, esb_listener_callback);
	while (true)
	{
        if(1 == update_flag){
            update_flag = 0;
            led_set_all(test_colors[0],test_colors[1],test_colors[2]);
            led_update();
        }

    }
}
