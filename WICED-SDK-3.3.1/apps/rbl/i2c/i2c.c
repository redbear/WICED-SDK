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
 * I2C Application
 *
 * This application demonstrates how to use the WICED I2C API
 * to read and write data from/to AT24C02 EEPROM
 *
 * Features demonstrated
 *  - I2C API
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
    wiced_i2c_device_t i2c_dev;
    uint8_t mem_addr = 0x00;
    uint8_t write_data[5] = {0x55, 0xAA, 0x5A, 0xA5, 0x00};
    uint8_t read_data[5];
    wiced_i2c_message_t i2c_tx_msg;
    wiced_i2c_message_t i2c_cmb_msg;
    uint8_t i;

    /* Initialise the WICED device */
    wiced_init();

    WPRINT_APP_INFO( ( "I2C API demonstration.\n" ) );

    i2c_dev.port = I2C_1;
    i2c_dev.address = 0x50;
    i2c_dev.address_width = I2C_ADDRESS_WIDTH_7BIT;
    i2c_dev.flags = I2C_DEVICE_NO_DMA;
    i2c_dev.speed_mode = I2C_STANDARD_SPEED_MODE;
    wiced_i2c_init( &i2c_dev );

    while ( 1 )
    {
        WPRINT_APP_INFO( ( "Write data to address %d :\n", mem_addr ) );
        for(i=0; i<sizeof(read_data); i++)
        {
            WPRINT_APP_INFO( ( "0x%02X, ", write_data[i] ) );
        }
        WPRINT_APP_INFO( ( "\n" ) );
        uint8_t temp_data[6];
        temp_data[0] = mem_addr;
        memcpy(&temp_data[1], write_data, sizeof(write_data));
        wiced_i2c_init_tx_message( &i2c_tx_msg, temp_data, sizeof(temp_data), 1, WICED_TRUE );
        wiced_i2c_transfer( &i2c_dev, &i2c_tx_msg, 1 );

        wiced_rtos_delay_milliseconds( 2000 );

        wiced_i2c_init_combined_message( &i2c_cmb_msg, &mem_addr, read_data, 1, sizeof(read_data), 1, WICED_TRUE );
        wiced_i2c_transfer( &i2c_dev, &i2c_cmb_msg, 1 );
        WPRINT_APP_INFO( ( "Read data from address %d :\n", mem_addr ) );
        for(i=0; i<sizeof(read_data); i++)
        {
            WPRINT_APP_INFO( ( "0x%02X, ", read_data[i] ) );
        }
        WPRINT_APP_INFO( ( "\n\n\n" ) );

        wiced_rtos_delay_milliseconds( 2000 );

        write_data[4]++;
    }
}
