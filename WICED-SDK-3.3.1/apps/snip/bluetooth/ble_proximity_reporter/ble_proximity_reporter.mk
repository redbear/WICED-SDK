#
# Copyright 2015, Broadcom Corporation
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom Corporation.
#

NAME := Bluetooth_Low_Energy_Proximity_Reporter_Application

$(NAME)_SOURCES    := ble_proximity_reporter.c \
                      wiced_bt_gatt_db.c \
                      wiced_bt_cfg.c

$(NAME)_INCLUDES   := .

$(NAME)_COMPONENTS := libraries/drivers/bluetooth

VALID_PLATFORMS += BCM9WCDPLUS114 \
                   BCM943909WCD1 \
                   BCM943909WCD1_3 \
                   BCM943341WCD1 \
                   BCM9WCD1AUDIO \
                   BCM943438WLPTH_2 \
                   RB_Duo \
                   RB_Duo_ES2
