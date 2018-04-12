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
 * I2C Application
 *
 * This application demonstrates how to use the WICED I2C API
 * to operate Grove OLED display 0.96"
 *
 * Features demonstrated
 *  - I2C API
 *
 */

#include "wiced.h"
#include "SeeedGrayOLED.h"

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

    WPRINT_APP_INFO( ( "I2C API demonstration.\n" ) );

    SeeedGrayOled_init();             // initialize SEEED OLED display
    SeeedGrayOled_clearDisplay();     // Clear Display
    SeeedGrayOled_setNormalDisplay(); // Set Normal Display Mode
    SeeedGrayOled_setVerticalMode();  // Set to vertical mode for displaying text
    SeeedGrayOled_setGrayLevel(15);   // Set Grayscale level. Any number between 0 - 15.

    SeeedGrayOled_setTextXY(3,0);
    SeeedGrayOled_putString("  HELLO     ");
    SeeedGrayOled_setTextXY(6,0);
    SeeedGrayOled_putString("      MAKER ");
    SeeedGrayOled_setTextXY(9,0);
    SeeedGrayOled_putString("   (*^_^*)  ");
}
