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
 * RGB Application
 *
 * This application demonstrates how to use the WICED GPIO API
 * to toggle the RGB and read button states
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

/******************************************************
 *               Variable Definitions
 ******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/

void application_start( )
{
    uint8_t      led_state = 0;
    wiced_bool_t button_pressed;

    /* Initialise the WICED device */
    wiced_init();

    // The RGB and setup button are initialized in platform.c

    WPRINT_APP_INFO( ( "The RGB are flashing R-G-B alternately. Holding the setup button will force it flashing white.\n" ) );

    while ( 1 )
    {
        /* Read the state of setup button */
        button_pressed = wiced_gpio_input_get( SETUP_BUTTON ) ? WICED_FALSE : WICED_TRUE;  /* The button has inverse logic */

        if ( button_pressed == WICED_TRUE )
        {
            /* Flashing white */
        	if( (led_state%2) == 0 )
        	{
				wiced_gpio_output_low( RGB_R );
				wiced_gpio_output_low( RGB_G );
				wiced_gpio_output_low( RGB_B );
        	}
        	else
        	{
        		wiced_gpio_output_high( RGB_R );
				wiced_gpio_output_high( RGB_G );
				wiced_gpio_output_high( RGB_B );
        	}
        }
        else
        {
            /* Flashing R-G-B alternately */
            if ( (led_state%3) == 0 )
            {
                wiced_gpio_output_low( RGB_R );
                wiced_gpio_output_high( RGB_G );
                wiced_gpio_output_high( RGB_B );
            }
            else if ( (led_state%3) == 1 )
            {
                wiced_gpio_output_high( RGB_R );
                wiced_gpio_output_low( RGB_G );
                wiced_gpio_output_high( RGB_B );
            }
            else
            {
            	wiced_gpio_output_high( RGB_R );
				wiced_gpio_output_high( RGB_G );
				wiced_gpio_output_low( RGB_B );
            }
        }

        wiced_rtos_delay_milliseconds( 300 );
        led_state++;
    }
}
