/*
 * Copyright 2014, RedBearLab Corporation
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of RedBearLab Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of RedBearLab Corporation.
 */

/** @file
 * Defines peripherals available for use on RBL205IOT board
 */
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
/*
Notes
1. These mappings are defined in <WICED-SDK>/platforms/RBL205RGAP6212/platform.c
2. STM32F2xx Datasheet  -> http://www.st.com/web/en/resource/technical/document/datasheet/CD00237391.pdf
3. STM32F2xx Ref Manual -> http://www.st.com/web/en/resource/technical/document/reference_manual/CD00225773.pdf
*/

/******************************************************
 *                   Enumerations
 ******************************************************/

typedef enum
{
   D0,
   D1,
   D2,
   D3,
   D4,
   D5,
   D6,
   D7,
   D8,
   D9,
   D10,
   D11,
   D12,
   D13,
   D14,
   D15,
   D16,
   D17,

   // LED & Button
   D18,
   D19,
   D20,
   D21,

   // SPI Flash
   FLASH_SPI_CS,
   FLASH_SPI_CLK,
   FLASH_SPI_MOSI,
   FLASH_SPI_MISO,

    WICED_GPIO_MAX, /* Denotes the total number of GPIO port aliases. Not a valid GPIO alias */
    WICED_GPIO_32BIT = 0x7FFFFFFF,
} wiced_gpio_t;

typedef enum
{
    A0,
    A1,
    A2,
    A3,
    A4,
    A5,
    A6,
    A7,

    WICED_ADC_MAX, /* Denotes the total number of ADC port aliases. Not a valid ADC alias */
    WICED_ADC_32BIT = 0x7FFFFFFF,
} wiced_adc_t;

typedef enum
{
    PWM0,
    PWM1,
    PWM2,
    PWM3,
    PWM4,
    PWM5,
    PWM6,
    PWM7,
    PWM8,
    PWM9,
    PWM10,
    PWM11,
    PWM12,

    PWM13,
    PWM14,
    PWM15,

    WICED_PWM_MAX, /* Denotes the total number of PWM port aliases. Not a valid PWM alias */
    WICED_PWM_32BIT = 0x7FFFFFFF,
} wiced_pwm_t;

typedef enum
{
    I2C_1,
    WICED_I2C_MAX,
    WICED_I2C_32BIT = 0x7FFFFFFF,
} wiced_i2c_t;

typedef enum
{
    SPI_1,
    SPI_2,
    SPI_3,
    WICED_SPI_MAX, /* Denotes the total number of SPI port aliases. Not a valid SPI alias */
    WICED_SPI_32BIT = 0x7FFFFFFF,
} wiced_spi_t;

typedef enum
{
    I2S_1,
    WICED_I2S_MAX, /* Denotes the total number of I2S port aliases.  Not a valid I2S alias */
    WICED_I2S_32BIT = 0x7FFFFFFF
} wiced_i2s_t;

typedef enum
{
    UART_1,
    UART_2,
    WICED_UART_MAX, /* Denotes the total number of UART port aliases. Not a valid UART alias */
    WICED_UART_32BIT = 0x7FFFFFFF,
} wiced_uart_t;

/******************************************************
 *                    Constants
 ******************************************************/

/* UART port used for standard I/O */
#define STDIO_UART ( UART_1 )

/* SPI flash is present */
#define WICED_PLATFORM_INCLUDES_SPI_FLASH
#define WICED_SPI_FLASH_CS FLASH_SPI_CS

#define DAC_EXTERNAL_OSCILLATOR

/* Components connected to external I/Os */
#define LED           ( D7  )
#if defined(ES0) || defined(ES2)
    #define LED_R         ( D18 )
    #define LED_G         ( D19 )
    #define LED_B         ( D20 )
#else
    #define LED_R         ( D20 )
    #define LED_G         ( D19 )
    #define LED_B         ( D18 )
#endif
#define BTN1          ( D21 )

/* I/O connection <-> Peripheral Connections */
#if defined(ES0) || defined(ES2)
    #define LED_R_JOINS_PWM        ( PWM15 )
    #define LED_G_JOINS_PWM        ( PWM14 )
    #define LED_B_JOINS_PWM        ( PWM13 )
#else
    #define LED_R_JOINS_PWM        ( PWM13 )
    #define LED_G_JOINS_PWM        ( PWM14 )
    #define LED_B_JOINS_PWM        ( PWM15 )
#endif

//#define WICED_THERMISTOR_JOINS_ADC  ( WICED_ADC_3 )

/* Components connected to external I/Os */
#define WICED_LED1         ( LED_R )
#define WICED_LED2         ( LED_G )
#define WICED_BUTTON1      ( BTN1 )
#define WICED_BUTTON2      ( WICED_GPIO_MAX )
#define WICED_THERMISTOR   ( WICED_GPIO_MAX )

/* I/O connection <-> Peripheral Connections */
#define WICED_LED1_JOINS_PWM        ( LED_R_JOINS_PWM )
#define WICED_LED2_JOINS_PWM        ( LED_G_JOINS_PWM )
//#define WICED_THERMISTOR_JOINS_ADC  ( WICED_ADC_3 )

/*  Bootloader LED D1 */
#define BOOTLOADER_LED_GPIO      ( LED_R )
#define BOOTLOADER_LED_ON_STATE  ( WICED_ACTIVE_LOW )

 /* Bootloader Button S1 */
#define BOOTLOADER_BUTTON_GPIO           ( BTN1 )
#define BOOTLOADER_BUTTON_PRESSED_STATE  ( WICED_ACTIVE_LOW )


#ifdef __cplusplus
} /*extern "C" */
#endif
