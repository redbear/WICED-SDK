#
# Broadcom Proprietary and Confidential. Copyright 2016 Broadcom
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom Corporation.
#

NAME :=                 Bluetooth_Low_Energy_Proximity_Reporter_Application

$(NAME)_SOURCES :=      ble_proximity_reporter.c \
                        wiced_bt_gatt_db.c \
                        wiced_bt_cfg.c

$(NAME)_INCLUDES :=     .

$(NAME)_COMPONENTS :=   libraries/drivers/bluetooth/low_energy

VALID_PLATFORMS +=      BCM9WCDPLUS114 \
                        BCM943909WCD* \
                        BCM943341WCD1 \
                        BCM9WCD1AUDIO \
                        BCM943438WLPTH_2 \
                        BCM943438WCD1 \
                        BCM94343WWCD1 \
                        BCM943907AEVAL2F_1 \
                        BCM958100SVK \
                        BCM943907WAE_1 \
                        RB_DUO \
                        RB_DUO_ext

INVALID_PLATFORMS +=    BCM943907WCD2
