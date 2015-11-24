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
 * Defines board support package for RBL205IOT board
 */
#include "platform.h"
#include "platform_config.h"
#include "platform_init.h"
#include "platform_isr.h"
#include "platform_peripheral.h"
#include "wwd_platform_common.h"
#include "wwd_rtos_isr.h"
#include "wiced_defaults.h"
#include "wiced_platform.h"
#include "platform_bluetooth.h"

/******************************************************
 *                      Macros
 ******************************************************/
#define PLATFORM_FACTORY_RESET_CHECK_PERIOD     ( 100 )
#define PLATFORM_FACTORY_RESET_TIMEOUT          ( 5000 )

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
    [ D0 ] = { GPIOB,  7 }, // I2C1_SDA
    [ D1 ] = { GPIOB,  6 }, // I2C1_SCL /CAN2_TX
    [ D2 ] = { GPIOB,  5 }, // SPI3_MOSI/CAN2_RX/I2S3_SD
    [ D3 ] = { GPIOB,  4 }, // SPI3_MISO
    [ D4 ] = { GPIOB,  3 }, // SPI3_SCK         /I2S3_SCK
    [ D5 ] = { GPIOA, 15 }, // SPI3_NSS         /I2S3_WS
    [ D6 ] = { GPIOA, 14 }, // SWCLK
    [ D7 ] = { GPIOA, 13 }, // SWDIO
    [ D8 ] = { GPIOA,  2 }, // ADC1_IN2/USART2_TX
    [ D9 ] = { GPIOA,  3 }, // ADC1_IN3/USART2_RX
    [ D10] = { GPIOA,  4 }, // ADC1_IN4/SPI1_NSS/DAC_OUT1/
    [ D11] = { GPIOA,  5 }, // ADC1_IN5/SPI1_SCK/DAC_OUT2
    [ D12] = { GPIOA,  6 }, // ADC1_IN6/SPI1_MISO
    [ D13] = { GPIOA,  7 }, // ADC1_IN7/SPI1_MOSI
    [ D14] = { GPIOA,  1 }, // ADC1_IN1/USART2_RTS
    [ D15] = { GPIOA,  0 }, // ADC1_IN0/USART2_CTS/WKUP
    [ D16] = { GPIOA, 10 }, // USART1_RX
    [ D17] = { GPIOA,  9 }, // USART1_TX

    // LEDs and Button
    [ D18] = { GPIOB,  0 },
    [ D19] = { GPIOB,  1 },
    [ D20] = { GPIOB, 11 },
    [ D21] = { GPIOB,  2 }, //BOOT1

    // SPI Flash
    [FLASH_SPI_CS  ] = { GPIOB, 12 },
    [FLASH_SPI_CLK ] = { GPIOB, 13 },
    [FLASH_SPI_MOSI] = { GPIOC,  3 },
    [FLASH_SPI_MISO] = { GPIOC,  2 },

    [WICED_GPIO_MAX]= { GPIOE,  0 }, //No this GPIO
};

/* ADC peripherals. Used WICED/platform/MCU/wiced_platform_common.c */
const platform_adc_t platform_adc_peripherals[] =
{
/*PA2 */ [ A0 ] = { ADC1, ADC_Channel_2,  RCC_APB2Periph_ADC1, 1, &platform_gpio_pins[ D8 ] },
/*PA3 */ [ A1 ] = { ADC1, ADC_Channel_3,  RCC_APB2Periph_ADC1, 1, &platform_gpio_pins[ D9 ] },
/*PA4 */ [ A2 ] = { ADC1, ADC_Channel_4,  RCC_APB2Periph_ADC1, 1, &platform_gpio_pins[ D10] },
/*PA5 */ [ A3 ] = { ADC1, ADC_Channel_5,  RCC_APB2Periph_ADC1, 1, &platform_gpio_pins[ D11] },
/*PA6 */ [ A4 ] = { ADC1, ADC_Channel_6,  RCC_APB2Periph_ADC1, 1, &platform_gpio_pins[ D12] },
/*PA7 */ [ A5 ] = { ADC1, ADC_Channel_7,  RCC_APB2Periph_ADC1, 1, &platform_gpio_pins[ D13] },
/*PA1 */ [ A6 ] = { ADC1, ADC_Channel_1,  RCC_APB2Periph_ADC1, 1, &platform_gpio_pins[ D14] },
/*PA0 */ [ A7 ] = { ADC1, ADC_Channel_0,  RCC_APB2Periph_ADC1, 1, &platform_gpio_pins[ D15] },
};

/* PWM peripherals. Used by WICED/platform/MCU/wiced_platform_common.c */
const platform_pwm_t platform_pwm_peripherals[] =
{
/*PB7 */ [ PWM0 ]  = { TIM4,  2, RCC_APB1Periph_TIM4,  GPIO_AF_TIM4,  &platform_gpio_pins[ D0 ] },
/*PB6 */ [ PWM1 ]  = { TIM4,  1, RCC_APB1Periph_TIM4,  GPIO_AF_TIM4,  &platform_gpio_pins[ D1 ] },
/*PB5 */ [ PWM2 ]  = { TIM3,  2, RCC_APB1Periph_TIM3,  GPIO_AF_TIM3,  &platform_gpio_pins[ D2 ] },
/*PB4 */ [ PWM3 ]  = { TIM3,  1, RCC_APB1Periph_TIM3,  GPIO_AF_TIM3,  &platform_gpio_pins[ D3 ] },
/*PB3 */ [ PWM4 ]  = { TIM2,  2, RCC_APB1Periph_TIM2,  GPIO_AF_TIM2,  &platform_gpio_pins[ D4 ] },
/*PA2 */ [ PWM5 ]  = { TIM5,  3, RCC_APB1Periph_TIM5,  GPIO_AF_TIM5,  &platform_gpio_pins[ D8 ] },//TIM2_CH3/TIM9_CH1
/*PA3 */ [ PWM6 ]  = { TIM5,  4, RCC_APB1Periph_TIM5,  GPIO_AF_TIM5,  &platform_gpio_pins[ D9 ] },//TIM2_CH4/TIM9_CH2
/*PA6 */ [ PWM7 ]  = { TIM13, 1, RCC_APB1Periph_TIM13, GPIO_AF_TIM13, &platform_gpio_pins[ D12] },//TIM3_CH1
/*PA7 */ [ PWM8 ]  = { TIM14, 1, RCC_APB1Periph_TIM14, GPIO_AF_TIM14, &platform_gpio_pins[ D13] },//TIM3_CH2
/*PA1 */ [ PWM9 ]  = { TIM5,  2, RCC_APB1Periph_TIM5,  GPIO_AF_TIM5,  &platform_gpio_pins[ D14] },//TIM2_CH2
/*PA0 */ [ PWM10]  = { TIM5,  1, RCC_APB1Periph_TIM5,  GPIO_AF_TIM5,  &platform_gpio_pins[ D15] },
/*PA10*/ [ PWM11]  = { TIM1,  3, RCC_APB2Periph_TIM1,  GPIO_AF_TIM1,  &platform_gpio_pins[ D16] },
/*PA9 */ [ PWM12]  = { TIM1,  2, RCC_APB2Periph_TIM1,  GPIO_AF_TIM1,  &platform_gpio_pins[ D17] },
/*PB0 */ [ PWM13]  = { TIM3,  3, RCC_APB1Periph_TIM3,  GPIO_AF_TIM3,  &platform_gpio_pins[ D18] },
/*PB1 */ [ PWM14]  = { TIM3,  4, RCC_APB1Periph_TIM3,  GPIO_AF_TIM3,  &platform_gpio_pins[ D19] },
/*PB11*/ [ PWM15]  = { TIM2,  4, RCC_APB1Periph_TIM2,  GPIO_AF_TIM2,  &platform_gpio_pins[ D20] },
};

/* I2C peripherals. Used by WICED/platform/MCU/wiced_platform_common.c */
const platform_i2c_t platform_i2c_peripherals[] =
{
    [I2C_1] =
    {
        .port                    = I2C1,
        .pin_scl                 = &platform_gpio_pins[D1], /*PB6 */
        .pin_sda                 = &platform_gpio_pins[D0], /*PB7 */
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
    [SPI_1]  =
    {
        .port                  = SPI1,
        .gpio_af               = GPIO_AF_SPI1,
        .peripheral_clock_reg  = RCC_APB2Periph_SPI1,
        .peripheral_clock_func = RCC_APB2PeriphClockCmd,
        .pin_mosi              = &platform_gpio_pins[D13], /*PA7 */
        .pin_miso              = &platform_gpio_pins[D12], /*PA6 */
        .pin_clock             = &platform_gpio_pins[D11], /*PA5 */
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

    [SPI_2]  =
    {
        .port                  = SPI2,
        .gpio_af               = GPIO_AF_SPI2,
        .peripheral_clock_reg  = RCC_APB1Periph_SPI2,
        .peripheral_clock_func = RCC_APB1PeriphClockCmd,
        .pin_mosi              = &platform_gpio_pins[FLASH_SPI_MOSI], /*PC3  */
        .pin_miso              = &platform_gpio_pins[FLASH_SPI_MISO], /*PC2  */
        .pin_clock             = &platform_gpio_pins[FLASH_SPI_CLK ], /*PB13 */
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

    [SPI_3]  =
    {
        .port                  = SPI3,
        .gpio_af               = GPIO_AF_SPI3,
        .peripheral_clock_reg  = RCC_APB1Periph_SPI3,
        .peripheral_clock_func = RCC_APB1PeriphClockCmd,
        .pin_mosi              = &platform_gpio_pins[D2], /*PB5 */
        .pin_miso              = &platform_gpio_pins[D3], /*PB4 */
        .pin_clock             = &platform_gpio_pins[D4], /*PB3 */
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
    .port        = SPI_2,
    .chip_select = WICED_SPI_FLASH_CS,
    .speed       = 50000000,
    .mode        = (SPI_CLOCK_RISING_EDGE | SPI_CLOCK_IDLE_HIGH | SPI_NO_DMA | SPI_MSB_FIRST),
    .bits        = 8
};
#endif

/* I2S peripherals */
const platform_i2s_t platform_i2s_peripherals[] =
{
    [I2S_1] =
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
        .pin_ck                       = &platform_gpio_pins[D4],     // Fake pins for temporary
        .pin_sd                       = &platform_gpio_pins[D2],
        .pin_ws                       = &platform_gpio_pins[D5],
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
    [UART_1] =
    {
        .port               = USART1,
        .tx_pin             = &platform_gpio_pins[D17], /*PA9 */
        .rx_pin             = &platform_gpio_pins[D16], /*PA10*/
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

    [UART_2] =
    {
        .port               = USART2,
        .tx_pin             = &platform_gpio_pins[D8], /*PA2 */
        .rx_pin             = &platform_gpio_pins[D9], /*PA3 */
        .cts_pin            = &platform_gpio_pins[D15], /*PA0 */
        .rts_pin            = &platform_gpio_pins[D14], /*PA1 */
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
    [WWD_PIN_RESET      ] = { GPIOC,  1 },
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
};

/******************************************************
 *               Function Definitions
 ******************************************************/

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
    //wiced_bool_t button1_pressed;

    /* Initialise buttons to input by default */
    platform_gpio_init( &platform_gpio_pins[BTN1], INPUT_PULL_UP );
#if 0
    button1_pressed = wiced_gpio_input_get( BTN1 ) ? WICED_FALSE : WICED_TRUE;  /* The button has inverse logic */
    if ( button1_pressed != WICED_TRUE )
    {
        platform_gpio_init( &platform_gpio_pins[LED], OUTPUT_PUSH_PULL );
        platform_gpio_output_high( &platform_gpio_pins[LED] );
    }
#endif

    /* Initialise LEDs and turn off by default */
    platform_gpio_init( &platform_gpio_pins[LED_R], OUTPUT_PUSH_PULL );
    platform_gpio_init( &platform_gpio_pins[LED_G], OUTPUT_PUSH_PULL );
    platform_gpio_init( &platform_gpio_pins[LED_B], OUTPUT_PUSH_PULL );
    platform_gpio_output_high( &platform_gpio_pins[LED_R] );
    platform_gpio_output_high( &platform_gpio_pins[LED_G] );
    platform_gpio_output_high( &platform_gpio_pins[LED_B] );

#ifndef WICED_DISABLE_STDIO
    /* Initialise UART standard I/O */
    platform_stdio_init( &platform_uart_drivers[STDIO_UART], &platform_uart_peripherals[STDIO_UART], &stdio_config );
#endif
}

/* Checks if a factory reset is requested */
wiced_bool_t platform_check_factory_reset( void )
{
    uint32_t factory_reset_counter = 0;
    int led_state = 0;
    while (  ( 0 == platform_gpio_input_get( &platform_gpio_pins[ WICED_BUTTON1 ] ) )
          && ( ( factory_reset_counter += PLATFORM_FACTORY_RESET_CHECK_PERIOD ) <= PLATFORM_FACTORY_RESET_TIMEOUT )
          && ( WICED_SUCCESS == (wiced_result_t)host_rtos_delay_milliseconds( PLATFORM_FACTORY_RESET_CHECK_PERIOD ) )
          )
    {
        /* Factory reset button is being pressed. */
        /* User Must press it for 5 seconds to ensure it was not accidental */
        /* Toggle LED every 100ms */

        if ( led_state == 0 )
        {
            platform_gpio_output_high( &platform_gpio_pins[ WICED_LED1 ] );
            led_state = 1;
        }
        else
        {
            platform_gpio_output_low( &platform_gpio_pins[ WICED_LED1 ] );
            led_state = 0;
        }
        if ( factory_reset_counter == 5000 )
        {
            return WICED_TRUE;
        }
    }
    return WICED_FALSE;
}

/******************************************************
 *           Interrupt Handler Definitions
 ******************************************************/
WWD_RTOS_DEFINE_ISR( usart1_irq )
{
    platform_uart_irq( &platform_uart_drivers[UART_1] );
}
WWD_RTOS_DEFINE_ISR( usart1_tx_dma_irq )
{
    platform_uart_tx_dma_irq( &platform_uart_drivers[UART_1] );
}
WWD_RTOS_DEFINE_ISR( usart1_rx_dma_irq )
{
    platform_uart_rx_dma_irq( &platform_uart_drivers[UART_1] );
}

WWD_RTOS_DEFINE_ISR( usart2_irq )
{
    platform_uart_irq( &platform_uart_drivers[UART_2] );
}
WWD_RTOS_DEFINE_ISR( usart2_tx_dma_irq )
{
    platform_uart_tx_dma_irq( &platform_uart_drivers[UART_2] );
}
WWD_RTOS_DEFINE_ISR( usart2_rx_dma_irq )
{
    platform_uart_rx_dma_irq( &platform_uart_drivers[UART_2] );
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

