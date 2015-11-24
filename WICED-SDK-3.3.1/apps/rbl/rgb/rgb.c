/*
 * Copyright 2014, Broadcom Corporation
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */

/** @file
 *
 * RGB Application
 *
 * This application demonstrates how to use the WICED GPIO API
 * to toggle LEDs and read button states
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
    wiced_bool_t led1 = WICED_FALSE;
    wiced_bool_t button1_pressed;

    /* Initialise the WICED device */
    wiced_init();

    WPRINT_APP_INFO( ( "The RGB LEDs are flashing. Holding a button will force the corresponding LED on.\n" ) );

    while ( 1 )
    {
        /* Read the state of Button 1 */
        button1_pressed = wiced_gpio_input_get( BTN1 ) ? WICED_FALSE : WICED_TRUE;  /* The button has inverse logic */

        if ( button1_pressed == WICED_TRUE )
        {
            /* Turn red LED on and turn green LED off */
            wiced_gpio_output_low( LED_R );
            wiced_gpio_output_high( LED_G );
            wiced_gpio_output_high( LED_B );
        }
        else
        {
            /* Turn red LED off and toggle green led */
            wiced_gpio_output_high( LED_R );
            if ( led1 == WICED_TRUE )
            {
                wiced_gpio_output_low( LED_B );
                wiced_gpio_output_high( LED_G );
                led1 = WICED_FALSE;
            }
            else
            {
                wiced_gpio_output_high( LED_B );
                wiced_gpio_output_low( LED_G );
                led1 = WICED_TRUE;
            }
        }

        wiced_rtos_delay_milliseconds( 300 );
    }
}
