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
 * Blink Application
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
    /* Initialise the WICED device */
    wiced_init();

    WPRINT_APP_INFO( ( "The LEDs are flashing.\n" ) );

    while ( 1 )
    {
        wiced_gpio_output_low( LED );
        wiced_rtos_delay_milliseconds( 200 );

        wiced_gpio_output_high( LED );
        wiced_rtos_delay_milliseconds( 200 );
    }
}
