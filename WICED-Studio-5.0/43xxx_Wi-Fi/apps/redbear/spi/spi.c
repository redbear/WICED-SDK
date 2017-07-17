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
 * SPI Application
 *
 * This application demonstrates how to use the WICED SPI API
 * to read and write data from/to SPI Flash
 *
 * Features demonstrated
 *  - SPI API
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
    wiced_spi_device_t spi_dev;
    wiced_spi_message_segment_t spi_msg;
    uint8_t tx_data[4] = {0x55, 0xAA, 0x5A, 0xA5};
    uint8_t rx_data[4];

    /* Initialise the WICED device */
    wiced_init();

    WPRINT_APP_INFO( ( "SPI API demonstration.\n" ) );

    spi_dev.port        = WICED_SPI_3;
    spi_dev.chip_select = D10;
    spi_dev.speed       = 10000000;
    spi_dev.mode        = (SPI_CLOCK_RISING_EDGE | SPI_CLOCK_IDLE_HIGH | SPI_NO_DMA | SPI_MSB_FIRST);
    spi_dev.bits        = 8;
    wiced_spi_init( &spi_dev );

    while ( 1 )
    {
        spi_msg.tx_buffer = tx_data;
        spi_msg.rx_buffer = rx_data;
        spi_msg.length = sizeof(tx_data);
        wiced_spi_transfer( &spi_dev, &spi_msg, 1 );

        wiced_rtos_delay_milliseconds( 2000 );
    }
}
