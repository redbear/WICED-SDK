#
# Copyright 2015, Broadcom Corporation
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom Corporation.
#

NAME := Bluetooth_Low_Energy_Hello_Sensor_Application

$(NAME)_SOURCES    := ble_hello_sensor.c \
                      wiced_bt_cfg.c

$(NAME)_INCLUDES   := .

$(NAME)_COMPONENTS := libraries/drivers/bluetooth_le \
                      utilities/command_console

VALID_PLATFORMS += BCM9WCDPLUS114 \
                   BCM943909WCD* \
                   BCM943341WCD1 \
                   BCM9WCD1AUDIO \
                   BCM943438WLPTH_2 \
                   RB_DUO \
                   RB_DUO_ext
