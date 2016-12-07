/*
 * Copyright 2015, RedBear Corporation
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of RedBear Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of RedBear Corporation.
 */

/** @file
 *
 * NeoPixel Application
 *
 * This application demonstrates how to use the WICED GPIO API
 * to light up an Adafruit NeoPixel RGB
 *
 * Features demonstrated
 *  - GPIO API
 *
 */

#include "wiced.h"

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Static Function Declarations
 ******************************************************/
static void RGB_Init(wiced_gpio_t pin);
static void RGB_Show(uint8_t r, uint8_t g, uint8_t b);

/******************************************************
 *               Variable Definitions
 ******************************************************/
static const platform_gpio_t *rgb_pin;

extern const platform_gpio_t platform_gpio_pins[];


/******************************************************
 *               Function Definitions
 ******************************************************/

void application_start( )
{
    /* Initialise the WICED device */
    wiced_init();

    WPRINT_APP_INFO( ( "The color of the RGB is changing every second.\n" ) );

    RGB_Init( D0 );

    while ( 1 )
    {
        RGB_Show(0xFF, 0x00, 0x00);
        wiced_rtos_delay_milliseconds( 500 );

        RGB_Show(0x00, 0xFF, 0x00);
        wiced_rtos_delay_milliseconds( 500 );

        RGB_Show(0x00, 0x00, 0xFF);
        wiced_rtos_delay_milliseconds( 500 );

        RGB_Show(0xFF, 0x00, 0xFF);
        wiced_rtos_delay_milliseconds( 500 );

        RGB_Show(0xFF, 0xFF, 0x00);
        wiced_rtos_delay_milliseconds( 500 );

        RGB_Show(0x00, 0xFF, 0xFF);
        wiced_rtos_delay_milliseconds( 500 );
    }
}

static void RGB_Init(wiced_gpio_t pin)
{
    rgb_pin =  &platform_gpio_pins[pin];
    wiced_gpio_init( pin, OUTPUT_PUSH_PULL );
}

static void RGB_Show(uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t rgb[3] = {g, r, b};
    uint8_t *p     = rgb;
    uint8_t *end   = p + 3;

    while (p < end)
    {
        uint8_t pix = *p++;
        uint8_t mask = 0x80;

        for(mask=0x80; mask; mask>>=1)
        {
            if(pix & mask)
            {
                // T1H 700ns
                rgb_pin->port->BSRRL = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRL = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRL = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRL = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRL = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRL = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRL = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRL = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRL = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRL = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRL = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRL = (uint16_t) ( 1 << (rgb_pin->pin_number) );

                // T1L 600ns
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
            }
            else
            {
                // T0H 350ns
                rgb_pin->port->BSRRL = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRL = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRL = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRL = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRL = (uint16_t) ( 1 << (rgb_pin->pin_number) );

                // T0L 800ns
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
                rgb_pin->port->BSRRH = (uint16_t) ( 1 << (rgb_pin->pin_number) );
            }
        }
    }
}

