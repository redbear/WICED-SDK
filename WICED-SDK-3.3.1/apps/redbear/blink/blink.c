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
 * Blink Application
 *
 * This application demonstrates how to use the WICED GPIO API
 * to toggle the user LED and read the setup button states
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
    /* Initialise the WICED device */
    wiced_init();

    // Since the user LED shares the same pin with SWDIO, config this pin as GPIO will result in disabling the SWD interface.
    // To download firmware successfully afterwards, keep pressing the setup button for 2 more seconds to skip the user LED initialization.
    WPRINT_APP_INFO( ( "Keep pressing the setup button for 2 seconds will not initialize the user LED.\n" ) );
    wiced_rtos_delay_milliseconds( 2000 );

    if(WICED_TRUE == wiced_gpio_input_get(SETUP_BUTTON))
    {
    	WPRINT_APP_INFO( ( "The user LED are flashing.\n" ) );

    	wiced_gpio_init(USER_LED, OUTPUT_PUSH_PULL);

    	while ( 1 )
    	{
    		wiced_gpio_output_low( USER_LED );
    		wiced_rtos_delay_milliseconds( 300 );

    		wiced_gpio_output_high( USER_LED );
    		wiced_rtos_delay_milliseconds( 300 );
    	}
    }
    else
    {
    	WPRINT_APP_INFO( ( "The user LED are not flashing.\n" ) );
    }
}
