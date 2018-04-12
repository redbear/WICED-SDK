/*
 * Copyright 2016, RedBear
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of RedBear;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of RedBear Corporation.
 */

/** @file
 * Defines board support package for RedBear Duo board
 */
#include "platform.h"
#include "platform_config.h"
#include "platform_init.h"
#include "platform_isr.h"
#include "platform_peripheral.h"
#include "platform_bluetooth.h"
#include "wwd_platform_common.h"
#include "wwd_rtos_isr.h"
#include "wiced_defaults.h"
#include "wiced_platform.h"
#include "platform_mfi.h"
#include "platform_button.h"
#include "gpio_button.h"

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
 *               Function Declarations
 ******************************************************/

/******************************************************
 *               Variables Definitions
 ******************************************************/

/* GPIO pin table. Used by WICED/platform/MCU/wiced_platform_common.c */
const platform_gpio_t platform_gpio_pins[] =
{
    [ WICED_GPIO_1 ] = { GPIOB,  7 }, // I2C1_SDA
    [ WICED_GPIO_2 ] = { GPIOB,  6 }, // I2C1_SCL /CAN2_TX
    [ WICED_GPIO_3 ] = { GPIOB,  5 }, // SPI3_MOSI/CAN2_RX/I2S3_SD
    [ WICED_GPIO_4 ] = { GPIOB,  4 }, // SPI3_MISO
    [ WICED_GPIO_5 ] = { GPIOB,  3 }, // SPI3_SCK         /I2S3_SCK
    [ WICED_GPIO_6 ] = { GPIOA, 15 }, // SPI3_NSS         /I2S3_WS
    [ WICED_GPIO_7 ] = { GPIOA, 14 }, // SWCLK
    [ WICED_GPIO_8 ] = { GPIOA, 13 }, // SWDIO/LED
    [ WICED_GPIO_9 ] = { GPIOA,  3 }, // ADC1_IN3/USART2_RX
    [ WICED_GPIO_10] = { GPIOA,  2 }, // ADC1_IN2/USART2_TX
    [ WICED_GPIO_11] = { GPIOA,  4 }, // ADC1_IN4/SPI1_NSS/DAC_OUT1/
    [ WICED_GPIO_12] = { GPIOA,  5 }, // ADC1_IN5/SPI1_SCK/DAC_OUT2
    [ WICED_GPIO_13] = { GPIOA,  6 }, // ADC1_IN6/SPI1_MISO
    [ WICED_GPIO_14] = { GPIOA,  7 }, // ADC1_IN7/SPI1_MOSI
    [ WICED_GPIO_15] = { GPIOA,  1 }, // ADC1_IN1/USART2_RTS
    [ WICED_GPIO_16] = { GPIOA,  0 }, // ADC1_IN0/USART2_CTS/WKUP
    [ WICED_GPIO_17] = { GPIOA, 10 }, // USART1_RX
    [ WICED_GPIO_18] = { GPIOA,  9 }, // USART1_TX

    // LEDs and Button
    [ WICED_GPIO_19] = { GPIOB, 11 }, // LED_R
    [ WICED_GPIO_20] = { GPIOB,  1 }, // LED_G
    [ WICED_GPIO_21] = { GPIOB,  0 }, // LED_B
    [ WICED_GPIO_22] = { GPIOB,  2 }, // SETUP button / BOOT1

    // SPI Flash
    [ WICED_GPIO_23] = { GPIOB, 12 },
    [ WICED_GPIO_24] = { GPIOB, 13 },
    [ WICED_GPIO_25] = { GPIOC,  3 },
    [ WICED_GPIO_26] = { GPIOC,  2 },

    [WICED_GPIO_MAX]= { GPIOE,  0 }, //No this GPIO
};

/* ADC peripherals. Used WICED/platform/MCU/wiced_platform_common.c */
const platform_adc_t platform_adc_peripherals[] =
{
/*PA3 */ [ WICED_ADC_1 ] = { ADC1, ADC_Channel_3,  RCC_APB2Periph_ADC1, 1, &platform_gpio_pins[ WICED_GPIO_9 ] },
/*PA2 */ [ WICED_ADC_2 ] = { ADC1, ADC_Channel_2,  RCC_APB2Periph_ADC1, 1, &platform_gpio_pins[ WICED_GPIO_10] },
/*PA4 */ [ WICED_ADC_3 ] = { ADC1, ADC_Channel_4,  RCC_APB2Periph_ADC1, 1, &platform_gpio_pins[ WICED_GPIO_11] },
/*PA5 */ [ WICED_ADC_4 ] = { ADC1, ADC_Channel_5,  RCC_APB2Periph_ADC1, 1, &platform_gpio_pins[ WICED_GPIO_12] },
/*PA6 */ [ WICED_ADC_5 ] = { ADC1, ADC_Channel_6,  RCC_APB2Periph_ADC1, 1, &platform_gpio_pins[ WICED_GPIO_13] },
/*PA7 */ [ WICED_ADC_6 ] = { ADC1, ADC_Channel_7,  RCC_APB2Periph_ADC1, 1, &platform_gpio_pins[ WICED_GPIO_14] },
/*PA1 */ [ WICED_ADC_7 ] = { ADC1, ADC_Channel_1,  RCC_APB2Periph_ADC1, 1, &platform_gpio_pins[ WICED_GPIO_15] },
/*PA0 */ [ WICED_ADC_8 ] = { ADC1, ADC_Channel_0,  RCC_APB2Periph_ADC1, 1, &platform_gpio_pins[ WICED_GPIO_16] },
};

/* PWM peripherals. Used by WICED/platform/MCU/wiced_platform_common.c */
const platform_pwm_t platform_pwm_peripherals[] =
{
/*PB7 */ [ WICED_PWM_1 ]  = { TIM4,  2, RCC_APB1Periph_TIM4,  GPIO_AF_TIM4,  &platform_gpio_pins[ WICED_GPIO_1 ] },
/*PB6 */ [ WICED_PWM_2 ]  = { TIM4,  1, RCC_APB1Periph_TIM4,  GPIO_AF_TIM4,  &platform_gpio_pins[ WICED_GPIO_2 ] },
/*PB5 */ [ WICED_PWM_3 ]  = { TIM3,  2, RCC_APB1Periph_TIM3,  GPIO_AF_TIM3,  &platform_gpio_pins[ WICED_GPIO_3 ] },
/*PB4 */ [ WICED_PWM_4 ]  = { TIM3,  1, RCC_APB1Periph_TIM3,  GPIO_AF_TIM3,  &platform_gpio_pins[ WICED_GPIO_4 ] },
/*PB3 */ [ WICED_PWM_5 ]  = { TIM2,  2, RCC_APB1Periph_TIM2,  GPIO_AF_TIM2,  &platform_gpio_pins[ WICED_GPIO_5 ] },
/*PA3 */ [ WICED_PWM_6 ]  = { TIM5,  4, RCC_APB1Periph_TIM5,  GPIO_AF_TIM5,  &platform_gpio_pins[ WICED_GPIO_9 ] },//TIM2_CH4/TIM9_CH2
/*PA2 */ [ WICED_PWM_7 ]  = { TIM5,  3, RCC_APB1Periph_TIM5,  GPIO_AF_TIM5,  &platform_gpio_pins[ WICED_GPIO_10] },//TIM2_CH3/TIM9_CH1
/*PA6 */ [ WICED_PWM_8 ]  = { TIM13, 1, RCC_APB1Periph_TIM13, GPIO_AF_TIM13, &platform_gpio_pins[ WICED_GPIO_13] },//TIM3_CH1
/*PA7 */ [ WICED_PWM_9 ]  = { TIM14, 1, RCC_APB1Periph_TIM14, GPIO_AF_TIM14, &platform_gpio_pins[ WICED_GPIO_14] },//TIM3_CH2
/*PA1 */ [ WICED_PWM_10]  = { TIM5,  2, RCC_APB1Periph_TIM5,  GPIO_AF_TIM5,  &platform_gpio_pins[ WICED_GPIO_15] },//TIM2_CH2
/*PA0 */ [ WICED_PWM_11]  = { TIM5,  1, RCC_APB1Periph_TIM5,  GPIO_AF_TIM5,  &platform_gpio_pins[ WICED_GPIO_16] },
/*PA10*/ [ WICED_PWM_12]  = { TIM1,  3, RCC_APB2Periph_TIM1,  GPIO_AF_TIM1,  &platform_gpio_pins[ WICED_GPIO_17] },
/*PA9 */ [ WICED_PWM_13]  = { TIM1,  2, RCC_APB2Periph_TIM1,  GPIO_AF_TIM1,  &platform_gpio_pins[ WICED_GPIO_18] },
/*PB11*/ [ WICED_PWM_14]  = { TIM2,  4, RCC_APB1Periph_TIM2,  GPIO_AF_TIM2,  &platform_gpio_pins[ WICED_GPIO_19] },
/*PB1 */ [ WICED_PWM_15]  = { TIM3,  4, RCC_APB1Periph_TIM3,  GPIO_AF_TIM3,  &platform_gpio_pins[ WICED_GPIO_20] },
/*PB0 */ [ WICED_PWM_16]  = { TIM3,  3, RCC_APB1Periph_TIM3,  GPIO_AF_TIM3,  &platform_gpio_pins[ WICED_GPIO_21] },
};

/* I2C peripherals. Used by WICED/platform/MCU/wiced_platform_common.c */
const platform_i2c_t platform_i2c_peripherals[] =
{
    [WICED_I2C_1] =
    {
        .port                    = I2C1,
        .pin_scl                 = &platform_gpio_pins[WICED_GPIO_2], /*PB6 */
        .pin_sda                 = &platform_gpio_pins[WICED_GPIO_1], /*PB7 */
        .peripheral_clock_reg    = RCC_APB1Periph_I2C1,
        .tx_dma                  = DMA1,
        .tx_dma_peripheral_clock = RCC_AHB1Periph_DMA1,
        .tx_dma_stream           = DMA1_Stream7, //DMA1_Stream6 optional
        .rx_dma_stream           = DMA1_Stream0, //DMA1_Stream5 optional
        .tx_dma_stream_id        = 7,
        .rx_dma_stream_id        = 0,
        .tx_dma_channel          = DMA_Channel_1,
        .rx_dma_channel          = DMA_Channel_1,
        .gpio_af                 = GPIO_AF_I2C1,
    },
};

/* PWM peripherals. Used by WICED/platform/MCU/wiced_platform_common.c */
const platform_spi_t platform_spi_peripherals[] =
{
    [WICED_SPI_1]  =
    {
        .port                  = SPI1,
        .gpio_af               = GPIO_AF_SPI1,
        .peripheral_clock_reg  = RCC_APB2Periph_SPI1,
        .peripheral_clock_func = RCC_APB2PeriphClockCmd,
        .pin_mosi              = &platform_gpio_pins[WICED_GPIO_14], /*PA7 */
        .pin_miso              = &platform_gpio_pins[WICED_GPIO_13], /*PA6 */
        .pin_clock             = &platform_gpio_pins[WICED_GPIO_12], /*PA5 */
        .tx_dma =
        {
            .controller        = DMA2,
            .stream            = DMA2_Stream5, //DMA2_Stream3 is optional
            .channel           = DMA_Channel_3,
            .irq_vector        = DMA2_Stream5_IRQn,
            .complete_flags    = DMA_HISR_TCIF5,
            .error_flags       = ( DMA_HISR_TEIF5 | DMA_HISR_FEIF5 | DMA_HISR_DMEIF5 ),
        },
        .rx_dma =
        {
            .controller        = DMA2,
            .stream            = DMA2_Stream0, //DMA2_Stream2 is optional
            .channel           = DMA_Channel_3,
            .irq_vector        = DMA2_Stream0_IRQn,
            .complete_flags    = DMA_LISR_TCIF0,
            .error_flags       = ( DMA_LISR_TEIF0 | DMA_LISR_FEIF0 | DMA_LISR_DMEIF0 ),
        },
    },

	/* Occupied by external serial flash */
    [WICED_SPI_2]  =
    {
        .port                  = SPI2,
        .gpio_af               = GPIO_AF_SPI2,
        .peripheral_clock_reg  = RCC_APB1Periph_SPI2,
        .peripheral_clock_func = RCC_APB1PeriphClockCmd,
        .pin_mosi              = &platform_gpio_pins[WICED_GPIO_25], /*PC3  */
        .pin_miso              = &platform_gpio_pins[WICED_GPIO_26], /*PC2  */
        .pin_clock             = &platform_gpio_pins[WICED_GPIO_24], /*PB13 */
        .tx_dma =
        {
            .controller        = DMA1,
            .stream            = DMA1_Stream4,
            .channel           = DMA_Channel_0,
            .irq_vector        = DMA1_Stream4_IRQn,
            .complete_flags    = DMA_HISR_TCIF4,
            .error_flags       = ( DMA_HISR_TEIF4 | DMA_HISR_FEIF4 | DMA_HISR_DMEIF4 ),
        },
        .rx_dma =
        {
            .controller        = DMA1,
            .stream            = DMA1_Stream3,
            .channel           = DMA_Channel_0,
            .irq_vector        = DMA1_Stream3_IRQn,
            .complete_flags    = DMA_LISR_TCIF3,
            .error_flags       = ( DMA_LISR_TEIF3 | DMA_LISR_FEIF3 | DMA_LISR_DMEIF3 ),
        },
    },

    [WICED_SPI_3]  =
    {
        .port                  = SPI3,
        .gpio_af               = GPIO_AF_SPI3,
        .peripheral_clock_reg  = RCC_APB1Periph_SPI3,
        .peripheral_clock_func = RCC_APB1PeriphClockCmd,
        .pin_mosi              = &platform_gpio_pins[WICED_GPIO_3], /*PB5 */
        .pin_miso              = &platform_gpio_pins[WICED_GPIO_4], /*PB4 */
        .pin_clock             = &platform_gpio_pins[WICED_GPIO_5], /*PB3 */
        .tx_dma =
        {
            .controller        = DMA1,
            .stream            = DMA1_Stream7, //DMA1_Stream5 is optional
            .channel           = DMA_Channel_0,
            .irq_vector        = DMA1_Stream7_IRQn,
            .complete_flags    = DMA_HISR_TCIF7,
            .error_flags       = ( DMA_HISR_TEIF7 | DMA_HISR_FEIF7 | DMA_HISR_DMEIF7 ),
        },
        .rx_dma =
        {
            .controller        = DMA1,
            .stream            = DMA1_Stream2, //DMA1_Stream0 is optional
            .channel           = DMA_Channel_0,
            .irq_vector        = DMA1_Stream2_IRQn,
            .complete_flags    = DMA_LISR_TCIF2,
            .error_flags       = ( DMA_LISR_TEIF2 | DMA_LISR_FEIF2 | DMA_LISR_DMEIF2 ),
        },
    }
};
platform_spi_slave_driver_t platform_spi_slave_drivers[WICED_SPI_MAX];

/* SPI flash. Exposed to the applications through include/wiced_platform.h */
#if defined ( WICED_PLATFORM_INCLUDES_SPI_FLASH )
const wiced_spi_device_t wiced_spi_flash =
{
    .port        = WICED_SPI_2,
    .chip_select = WICED_SPI_FLASH_CS,
    .speed       = 50000000,
    .mode        = (SPI_CLOCK_RISING_EDGE | SPI_CLOCK_IDLE_HIGH | SPI_NO_DMA | SPI_MSB_FIRST),
    .bits        = 8
};
#endif

/* I2S peripherals */
const platform_i2s_t platform_i2s_peripherals[] =
{
    [WICED_I2S_1] =
    {
        .spi                          = SPI3,
#ifdef DAC_EXTERNAL_OSCILLATOR
        .is_master                    = 0,
        .enable_mclk                  = 0,
#else /* DAC_EXTERNAL_OSCILLATOR */
        .is_master                    = 1,
        .enable_mclk                  = 1,
#endif /* DAC_EXTERNAL_OSCILLATOR */
        .gpio_af                      = GPIO_AF_SPI3,
        .peripheral_clock             = RCC_APB1Periph_SPI3,
        .peripheral_clock_func        = RCC_APB1PeriphClockCmd,
        .pin_ck                       = &platform_gpio_pins[WICED_GPIO_5],     // Fake pins for temporary
        .pin_sd                       = &platform_gpio_pins[WICED_GPIO_3],
        .pin_ws                       = &platform_gpio_pins[WICED_GPIO_6],
        .pin_mclk                     = &platform_gpio_pins[WICED_GPIO_MAX],
        .tx_dma.dma_register          = DMA1,
        .tx_dma.stream                = DMA1_Stream7,
        .tx_dma.channel               = DMA_Channel_0,
        .tx_dma.peripheral_clock      = RCC_AHB1Periph_DMA1,
        .tx_dma.peripheral_clock_func = RCC_AHB1PeriphClockCmd,
        .tx_dma.irq                   = DMA1_Stream7_IRQn,
        .rx_dma.dma_register          = DMA1,
        .rx_dma.stream                = DMA1_Stream2,
        .rx_dma.channel               = DMA_Channel_0,
        .rx_dma.peripheral_clock      = RCC_AHB1Periph_DMA1,
        .rx_dma.peripheral_clock_func = RCC_AHB1PeriphClockCmd,
        .rx_dma.irq                   = DMA1_Stream2_IRQn,
    }
};

/* UART peripherals and runtime drivers. Used by WICED/platform/MCU/wiced_platform_common.c */
const platform_uart_t platform_uart_peripherals[] =
{
    [WICED_UART_1] =
    {
        .port               = USART1,
        .tx_pin             = &platform_gpio_pins[WICED_GPIO_18], /*PA9 */
        .rx_pin             = &platform_gpio_pins[WICED_GPIO_17], /*PA10*/
        .cts_pin            = NULL,
        .rts_pin            = NULL,
        .tx_dma_config =
        {
            .controller     = DMA2,
            .stream         = DMA2_Stream7,
            .channel        = DMA_Channel_4,
            .irq_vector     = DMA2_Stream7_IRQn,
            .complete_flags = DMA_HISR_TCIF7,
            .error_flags    = ( DMA_HISR_TEIF7 | DMA_HISR_FEIF7 | DMA_HISR_DMEIF7),
        },
        .rx_dma_config =
        {
            .controller     = DMA2,
            .stream         = DMA2_Stream2, //DMA2_Stream5 is optional
            .channel        = DMA_Channel_4,
            .irq_vector     = DMA2_Stream2_IRQn,
            .complete_flags = DMA_LISR_TCIF2,
            .error_flags    = ( DMA_LISR_TEIF2 | DMA_LISR_FEIF2 | DMA_LISR_DMEIF2 ),
        },
    },

    [WICED_UART_2] =
    {
        .port               = USART2,
        .tx_pin             = &platform_gpio_pins[WICED_GPIO_10], /*PA2 */
        .rx_pin             = &platform_gpio_pins[WICED_GPIO_9 ], /*PA3 */
        .cts_pin            = &platform_gpio_pins[WICED_GPIO_16], /*PA0 */
        .rts_pin            = &platform_gpio_pins[WICED_GPIO_15], /*PA1 */
        .tx_dma_config =
        {
            .controller     = DMA1,
            .stream         = DMA1_Stream6,
            .channel        = DMA_Channel_4,
            .irq_vector     = DMA1_Stream6_IRQn,
            .complete_flags = DMA_HISR_TCIF6,
            .error_flags    = ( DMA_HISR_TEIF6 | DMA_HISR_FEIF6 | DMA_HISR_DMEIF6 ),
        },
        .rx_dma_config =
        {
            .controller     = DMA1,
            .stream         = DMA1_Stream5,
            .channel        = DMA_Channel_4,
            .irq_vector     = DMA1_Stream5_IRQn,
            .complete_flags = DMA_HISR_TCIF5,
            .error_flags    = ( DMA_HISR_TEIF5 | DMA_HISR_FEIF5 | DMA_HISR_DMEIF5 ),
        },
    },
};
platform_uart_driver_t platform_uart_drivers[WICED_UART_MAX];

/* UART standard I/O configuration */
#ifndef WICED_DISABLE_STDIO
static platform_uart_config_t stdio_config =
{
    .baud_rate    = 115200,
    .data_width   = DATA_WIDTH_8BIT,
    .parity       = NO_PARITY,
    .stop_bits    = STOP_BITS_1,
    .flow_control = FLOW_CONTROL_DISABLED,
};
#endif

/* Wi-Fi control pins. Used by WICED/platform/MCU/wwd_platform_common.c
 * SDIO: WWD_PIN_BOOTSTRAP[1:0] = b'X0
 * gSPI: WWD_PIN_BOOTSTRAP[1:0] = b'01
 */
const platform_gpio_t wifi_control_pins[] =
{
    [WWD_PIN_POWER      ] = { GPIOC,  1 },
#if defined ( WICED_USE_WIFI_32K_CLOCK_MCO )
    [WWD_PIN_32K_CLK    ] = { GPIOA,  8 },
#else
    [WWD_PIN_32K_CLK    ] = { GPIOE,  2 },
#endif
    [WWD_PIN_BOOTSTRAP_0] = { GPIOE,  0 }, //No define it, Fake it
    [WWD_PIN_BOOTSTRAP_1] = { GPIOE,  1 }, //No define it, Fake it
};

/* Wi-Fi SDIO bus pins. Used by WICED/platform/STM32F2xx/WWD/wwd_SDIO.c */
const platform_gpio_t wifi_sdio_pins[] =
{
    [WWD_PIN_SDIO_OOB_IRQ] = { GPIOE,  0 }, //No define it, Fake it
    [WWD_PIN_SDIO_CLK    ] = { GPIOC, 12 },
    [WWD_PIN_SDIO_CMD    ] = { GPIOD,  2 },
    [WWD_PIN_SDIO_D0     ] = { GPIOC,  8 },
    [WWD_PIN_SDIO_D1     ] = { GPIOC,  9 },
    [WWD_PIN_SDIO_D2     ] = { GPIOC, 10 },
    [WWD_PIN_SDIO_D3     ] = { GPIOC, 11 },
};

/* Bluetooth control pins. Used by libraries/bluetooth/internal/bus/UART/bt_bus.c */
static const platform_gpio_t internal_bt_control_pins[] =
{
    /* Reset pin unavailable */
    [WICED_BT_PIN_POWER      ] = { GPIOC,  4 },
    [WICED_BT_PIN_HOST_WAKE  ] = { GPIOC,  0 },
    [WICED_BT_PIN_DEVICE_WAKE] = { GPIOC,  5 }
};
const platform_gpio_t* wiced_bt_control_pins[] =
{
    /* Reset pin unavailable */
    [WICED_BT_PIN_POWER      ] = &internal_bt_control_pins[WICED_BT_PIN_POWER      ],
    [WICED_BT_PIN_HOST_WAKE  ] = &internal_bt_control_pins[WICED_BT_PIN_HOST_WAKE  ],
    [WICED_BT_PIN_DEVICE_WAKE] = &internal_bt_control_pins[WICED_BT_PIN_DEVICE_WAKE],
};

/* Bluetooth UART pins. Used by libraries/bluetooth/internal/bus/UART/bt_bus.c */
static const platform_gpio_t internal_bt_uart_pins[] =
{
    [WICED_BT_PIN_UART_TX ] = { GPIOC,  6 },
    [WICED_BT_PIN_UART_RX ] = { GPIOC,  7 },
    [WICED_BT_PIN_UART_CTS] = { GPIOB,  9 },
    [WICED_BT_PIN_UART_RTS] = { GPIOB,  8 },
};
const platform_gpio_t* wiced_bt_uart_pins[] =
{
    [WICED_BT_PIN_UART_TX ] = &internal_bt_uart_pins[WICED_BT_PIN_UART_TX ],
    [WICED_BT_PIN_UART_RX ] = &internal_bt_uart_pins[WICED_BT_PIN_UART_RX ],
    [WICED_BT_PIN_UART_CTS] = &internal_bt_uart_pins[WICED_BT_PIN_UART_CTS],
    [WICED_BT_PIN_UART_RTS] = &internal_bt_uart_pins[WICED_BT_PIN_UART_RTS],
};

/* Bluetooth UART peripheral and runtime driver. Used by libraries/bluetooth/internal/bus/UART/bt_bus.c */
static const platform_uart_t internal_bt_uart_peripheral =
{
    .port               = USART6,
    .tx_pin             = &internal_bt_uart_pins[WICED_BT_PIN_UART_TX],
    .rx_pin             = &internal_bt_uart_pins[WICED_BT_PIN_UART_RX],
    .cts_pin            = &internal_bt_uart_pins[WICED_BT_PIN_UART_CTS],
    .rts_pin            = &internal_bt_uart_pins[WICED_BT_PIN_UART_RTS],
    .tx_dma_config =
    {
        .controller     = DMA2,
        .stream         = DMA2_Stream6,
        .channel        = DMA_Channel_5,
        .irq_vector     = DMA2_Stream6_IRQn,
        .complete_flags = DMA_HISR_TCIF6,
        .error_flags    = ( DMA_HISR_TEIF6 | DMA_HISR_FEIF6 | DMA_HISR_DMEIF6 ),
    },
    .rx_dma_config =
    {
        .controller     = DMA2,
        .stream         = DMA2_Stream1,
        .channel        = DMA_Channel_5,
        .irq_vector     = DMA2_Stream1_IRQn,
        .complete_flags = DMA_LISR_TCIF1,
        .error_flags    = ( DMA_LISR_TEIF1 | DMA_LISR_FEIF1 | DMA_LISR_DMEIF1 ),
    },
};
static platform_uart_driver_t internal_bt_uart_driver;
const platform_uart_t*        wiced_bt_uart_peripheral = &internal_bt_uart_peripheral;
platform_uart_driver_t*       wiced_bt_uart_driver     = &internal_bt_uart_driver;

/* Bluetooth UART configuration. Used by libraries/bluetooth/internal/bus/UART/bt_bus.c */
const platform_uart_config_t wiced_bt_uart_config =
{
    .baud_rate    = 115200,
    .data_width   = DATA_WIDTH_8BIT,
    .parity       = NO_PARITY,
    .stop_bits    = STOP_BITS_1,
    .flow_control = FLOW_CONTROL_DISABLED,
};

/*BT chip specific configuration information*/
const platform_bluetooth_config_t wiced_bt_config =
{
    .patchram_download_mode      = PATCHRAM_DOWNLOAD_MODE_MINIDRV_CMD,
    .patchram_download_baud_rate = 115200,
    .featured_baud_rate          = 115200
};

gpio_button_t platform_gpio_buttons[] =
{
    [PLATFORM_BUTTON_1] =
    {
        .polarity   = WICED_ACTIVE_LOW,
        .gpio       = WICED_BUTTON1,
        .trigger    = IRQ_TRIGGER_BOTH_EDGES,
    },
};

const wiced_gpio_t platform_gpio_leds[PLATFORM_LED_COUNT] =
{
     [WICED_LED_INDEX_1] = WICED_LED1,
     [WICED_LED_INDEX_2] = WICED_LED2,
     [WICED_LED_INDEX_3] = WICED_LED3,
};

#if 0
/* MFI-related variables */
const wiced_i2c_device_t auth_chip_i2c_device =
{
    .port          = WICED_I2C_1,
    .address       = 0x11,
    .address_width = I2C_ADDRESS_WIDTH_7BIT,
    .speed_mode    = I2C_STANDARD_SPEED_MODE,
};

const platform_mfi_auth_chip_t platform_auth_chip =
{
    .i2c_device = &auth_chip_i2c_device,
    .reset_pin  = WICED_GPIO_NONE
};
#endif

/******************************************************
 *               Function Definitions
 ******************************************************/

/* LEDs on this platform are active LOW */
platform_result_t platform_led_set_state(int led_index, int off_on )
{
    if ((led_index >= 0) && (led_index < PLATFORM_LED_COUNT))
    {
        switch (off_on)
        {
            case WICED_LED_OFF:
                platform_gpio_output_high( &platform_gpio_pins[platform_gpio_leds[led_index]] );
                break;
            case WICED_LED_ON:
                platform_gpio_output_low( &platform_gpio_pins[platform_gpio_leds[led_index]] );
                break;
        }
        return PLATFORM_SUCCESS;
    }
    return PLATFORM_BADARG;
}

void platform_led_init( void )
{
    /* Initialise LEDs and turn off by default */
    platform_gpio_init( &platform_gpio_pins[WICED_LED1], OUTPUT_PUSH_PULL );
    platform_gpio_init( &platform_gpio_pins[WICED_LED2], OUTPUT_PUSH_PULL );
    platform_gpio_init( &platform_gpio_pins[WICED_LED3], OUTPUT_PUSH_PULL );
    platform_led_set_state(WICED_LED_INDEX_1, WICED_LED_OFF);
    platform_led_set_state(WICED_LED_INDEX_2, WICED_LED_OFF);
    platform_led_set_state(WICED_LED_INDEX_3, WICED_LED_OFF);
 }

void platform_init_peripheral_irq_priorities( void )
{
    /* Interrupt priority setup. Called by WICED/platform/MCU/STM32F2xx/platform_init.c */
    NVIC_SetPriority( RTC_WKUP_IRQn    ,  1 ); /* RTC Wake-up event   */
    NVIC_SetPriority( SDIO_IRQn        ,  2 ); /* WLAN SDIO           */
    NVIC_SetPriority( DMA2_Stream3_IRQn,  3 ); /* WLAN SDIO DMA       */
    NVIC_SetPriority( USART6_IRQn      ,  6 ); /* Bluetooth UART      */
    NVIC_SetPriority( USART1_IRQn      ,  6 ); /* WICED_UART_1        */
    NVIC_SetPriority( USART2_IRQn      ,  6 ); /* WICED_UART_2        */
    NVIC_SetPriority( USART3_IRQn      ,  6 ); /* WICED_UART_3        */
    NVIC_SetPriority( DMA2_Stream6_IRQn,  7 ); /* Bluetooth TX DMA    */
    NVIC_SetPriority( DMA2_Stream1_IRQn,  7 ); /* Bluetooth RX DMA    */
    NVIC_SetPriority( DMA2_Stream7_IRQn,  7 ); /* WICED_UART_1 TX DMA */
    NVIC_SetPriority( DMA2_Stream2_IRQn,  7 ); /* WICED_UART_1 RX DMA */
    NVIC_SetPriority( DMA1_Stream6_IRQn,  7 ); /* WICED_UART_2 TX DMA */
    NVIC_SetPriority( DMA1_Stream5_IRQn,  7 ); /* WICED_UART_2 RX DMA */
    NVIC_SetPriority( DMA1_Stream4_IRQn,  7 ); /* WICED_UART_3 TX DMA */
    NVIC_SetPriority( DMA1_Stream1_IRQn,  7 ); /* WICED_UART_3 RX DMA */
    NVIC_SetPriority( EXTI0_IRQn       , 14 ); /* GPIO                */
    NVIC_SetPriority( EXTI1_IRQn       , 14 ); /* GPIO                */
    NVIC_SetPriority( EXTI2_IRQn       , 14 ); /* GPIO                */
    NVIC_SetPriority( EXTI3_IRQn       , 14 ); /* GPIO                */
    NVIC_SetPriority( EXTI4_IRQn       , 14 ); /* GPIO                */
    NVIC_SetPriority( EXTI9_5_IRQn     , 14 ); /* GPIO                */
    NVIC_SetPriority( EXTI15_10_IRQn   , 14 ); /* GPIO                */
}

void platform_init_external_devices( void )
{
    /* Initialise buttons to input by default */
    platform_gpio_init( &platform_gpio_pins[WICED_BUTTON1], INPUT_PULL_UP );

    /* Initialise LEDs and turn off by default */
    platform_led_init();

#ifndef WICED_DISABLE_STDIO
    /* Initialise UART standard I/O */
    platform_stdio_init( &platform_uart_drivers[STDIO_UART], &platform_uart_peripherals[STDIO_UART], &stdio_config );
#endif
}

/* Checks if a factory reset is requested */
uint32_t  platform_get_factory_reset_button_time ( uint32_t max_time )
{
    uint32_t button_press_timer = 0;
    int led_state = 0;

    /* Initialise input */
     platform_gpio_init( &platform_gpio_pins[ PLATFORM_FACTORY_RESET_BUTTON_GPIO ], INPUT_PULL_UP );

     while ( (PLATFORM_FACTORY_RESET_PRESSED_STATE == platform_gpio_input_get(&platform_gpio_pins[ PLATFORM_FACTORY_RESET_BUTTON_GPIO ])) )
    {
         /* How long is the "Factory Reset" button being pressed. */
         host_rtos_delay_milliseconds( PLATFORM_FACTORY_RESET_CHECK_PERIOD );

         /* Toggle LED every PLATFORM_FACTORY_RESET_CHECK_PERIOD  */
        if ( led_state == 0 )
        {
            platform_gpio_output_high( &platform_gpio_pins[ PLATFORM_FACTORY_RESET_LED_GPIO ] );
            led_state = 1;
        }
        else
        {
            platform_gpio_output_low( &platform_gpio_pins[ PLATFORM_FACTORY_RESET_LED_GPIO ] );
            led_state = 0;
        }

        button_press_timer += PLATFORM_FACTORY_RESET_CHECK_PERIOD;
        if ((max_time > 0) && (button_press_timer >= max_time))
        {
            break;
        }
    }

     /* turn off the LED */
     if (PLATFORM_FACTORY_RESET_LED_ON_STATE == WICED_ACTIVE_HIGH)
     {
         platform_gpio_output_low( &platform_gpio_pins[ PLATFORM_FACTORY_RESET_LED_GPIO ] );
     }
     else
     {
         platform_gpio_output_high( &platform_gpio_pins[ PLATFORM_FACTORY_RESET_LED_GPIO ] );
     }

    return button_press_timer;
}

/******************************************************
 *           Interrupt Handler Definitions
 ******************************************************/
WWD_RTOS_DEFINE_ISR( usart1_irq )
{
    platform_uart_irq( &platform_uart_drivers[WICED_UART_1] );
}
WWD_RTOS_DEFINE_ISR( usart1_tx_dma_irq )
{
    platform_uart_tx_dma_irq( &platform_uart_drivers[WICED_UART_1] );
}
WWD_RTOS_DEFINE_ISR( usart1_rx_dma_irq )
{
    platform_uart_rx_dma_irq( &platform_uart_drivers[WICED_UART_1] );
}

WWD_RTOS_DEFINE_ISR( usart2_irq )
{
    platform_uart_irq( &platform_uart_drivers[WICED_UART_2] );
}
WWD_RTOS_DEFINE_ISR( usart2_tx_dma_irq )
{
    platform_uart_tx_dma_irq( &platform_uart_drivers[WICED_UART_2] );
}
WWD_RTOS_DEFINE_ISR( usart2_rx_dma_irq )
{
    platform_uart_rx_dma_irq( &platform_uart_drivers[WICED_UART_2] );
}

WWD_RTOS_DEFINE_ISR( bt_uart_irq )
{
    platform_uart_irq( wiced_bt_uart_driver );
}
WWD_RTOS_DEFINE_ISR( bt_uart_tx_dma_irq )
{
    platform_uart_tx_dma_irq( wiced_bt_uart_driver );
}
WWD_RTOS_DEFINE_ISR( bt_uart_rx_dma_irq )
{
    platform_uart_rx_dma_irq( wiced_bt_uart_driver );
}

/******************************************************
 *            Interrupt Handlers Mapping
 ******************************************************/

/* These DMA assignments can be found STM32F2xx datasheet DMA section */
WWD_RTOS_MAP_ISR( usart1_irq       , USART1_irq       )
WWD_RTOS_MAP_ISR( usart1_tx_dma_irq, DMA2_Stream7_irq )
WWD_RTOS_MAP_ISR( usart1_rx_dma_irq, DMA2_Stream2_irq )

WWD_RTOS_MAP_ISR( usart2_irq       , USART2_irq       )
WWD_RTOS_MAP_ISR( usart2_tx_dma_irq, DMA1_Stream6_irq )
WWD_RTOS_MAP_ISR( usart2_rx_dma_irq, DMA1_Stream5_irq )

WWD_RTOS_MAP_ISR( bt_uart_irq        , USART6_irq       )
WWD_RTOS_MAP_ISR( bt_uart_tx_dma_irq , DMA2_Stream6_irq )
WWD_RTOS_MAP_ISR( bt_uart_rx_dma_irq , DMA2_Stream1_irq )

