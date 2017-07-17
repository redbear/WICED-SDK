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
wiced_adc_t analog_channel[8] = {A0, A1, A2, A3, A4, A5, A6, A7};
uint16_t    analog_value[8];

/******************************************************
 *               Function Definitions
 ******************************************************/

void application_start( )
{
    /* Initialise the WICED device */
    wiced_init();

    while ( 1 )
    {
        WPRINT_APP_INFO( ( "Analog input voltage measurement:\n" ) );
		
        for(uint8_t i=0; i<8; i++)
        {
            wiced_adc_init( analog_channel[i], 480 );
            // This function only takes sample for the channel that selected by the last time invoking wiced_adc_init().
            // So if you change to take sample for another channel, you need to invoke wiced_adc_init() to select this channel first.
            wiced_adc_take_sample( analog_channel[i], &analog_value[i] );
            WPRINT_APP_INFO( ( "Channel %d input voltage: %d\n", i, analog_value[i] ) );
    	}

        wiced_rtos_delay_milliseconds( 3000 );
        WPRINT_APP_INFO( ( "\n\n" ) );
    }
}
