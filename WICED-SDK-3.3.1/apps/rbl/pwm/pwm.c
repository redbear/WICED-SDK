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
 * NeoPixel Application
 *
 * This application demonstrates how to use the WICED PWM API
 * to light up a RGB
 *
 * Features demonstrated
 *  - PWM API
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
    float temp  = 100;
    float duty = 100;
    float cnt = 0;
    wiced_bool_t rise = WICED_FALSE;

    /* Initialise the WICED device */
    wiced_init();

    WPRINT_APP_INFO( ( "PWM test started.\n" ) );

    while ( 1 )
    {
        wiced_pwm_init( PWM14, 1000, duty );
        wiced_pwm_start( PWM14 );

        wiced_rtos_delay_milliseconds( 10 );

        if(rise)
        {
            temp += cnt;
            cnt -= 0.005;
            if(temp >= 100.0)
            {
                duty = 100;
                cnt = 0;
                rise = WICED_FALSE;
            }
            else
            {
                duty = temp;
            }
        }
        else
        {
            cnt += 0.005;
            temp -= cnt;
            if(temp <= 0.0)
            {
                duty = 0;
                rise = WICED_TRUE;
            }
            else
            {
                duty = temp;
            }
        }
    }
}
