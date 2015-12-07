/*
 * Copyright 2015, RedBear
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of RedBear;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of RedBear Corporation.
 */

/** @file
 * Defines Arduino compatible macro definitions for RedBear Duo.
 */
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#define D0                WICED_GPIO_1
#define D1                WICED_GPIO_2
#define D2                WICED_GPIO_3
#define D3                WICED_GPIO_4
#define D4                WICED_GPIO_5
#define D5                WICED_GPIO_6
#define D6                WICED_GPIO_7
#define D7                WICED_GPIO_8
#define D8                WICED_GPIO_9
#define D9                WICED_GPIO_10
#define D10               WICED_GPIO_11
#define D11               WICED_GPIO_12
#define D12               WICED_GPIO_13
#define D13               WICED_GPIO_14
#define D14               WICED_GPIO_15
#define D15               WICED_GPIO_16
#define D16               WICED_GPIO_17
#define D17               WICED_GPIO_18

#define A0                WICED_ADC_1
#define A1                WICED_ADC_2
#define A2                WICED_ADC_3
#define A3                WICED_ADC_4
#define A4                WICED_ADC_5
#define A5                WICED_ADC_6
#define A6                WICED_ADC_7
#define A7                WICED_ADC_8

#define PWM0              WICED_PWM_1
#define PWM1              WICED_PWM_2
#define PWM2              WICED_PWM_3
#define PWM3              WICED_PWM_4
#define PWM4              WICED_PWM_5
#define PWM5              WICED_PWM_6
#define PWM6              WICED_PWM_7
#define PWM7              WICED_PWM_8
#define PWM8              WICED_PWM_9
#define PWM9              WICED_PWM_10
#define PWM10             WICED_PWM_11
#define PWM11             WICED_PWM_12
#define PWM12             WICED_PWM_13
#define PWM13             WICED_PWM_14
#define PWM14             WICED_PWM_15
#define PWM15             WICED_PWM_16

#define RGB_R             WICED_LED1
#define RGB_G             WICED_LED2
#define RGB_B             WICED_LED3
#define USER_LED          WICED_LED4

#define RGB_R_PWM         PWM13
#define RGB_G_PWM         PWM14
#define RGB_B_PWM         PWM15

#define SETUP_BUTTON      WICED_BUTTON1


#ifdef __cplusplus
} /*extern "C" */
#endif
