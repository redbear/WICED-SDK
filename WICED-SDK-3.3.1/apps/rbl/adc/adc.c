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
 * Analog in Application
 *
 * This application demonstrates how to use the WICED ADC API
 * measure the analog input voltage
 *
 * Features demonstrated
 *  - ADC API
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
    uint16_t value;

    /* Initialise the WICED device */
    wiced_init();

    WPRINT_APP_INFO( ( "Analog in measurement...\n" ) );

    wiced_adc_init( A7, 480 );

    while ( 1 )
    {
        wiced_adc_take_sample( A7, &value );
        WPRINT_APP_INFO( ( "Analog in: %d\n", value ) );

        wiced_rtos_delay_milliseconds( 1000 );
    }
}
