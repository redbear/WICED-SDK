#
# Copyright 2015, Broadcom Corporation
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom Corporation.
#

NAME := WICED_BLUETOOTH_DUAL_MODE_SERVER

GLOBAL_DEFINES     := BT_TRACE_PROTOCOL=FALSE BT_USE_TRACES=FALSE

$(NAME)_SOURCES :=  \
            bt_dualmode_server.c \
            ble_proximity_reporter.c \
            wiced_bt_gatt_db.c \
            wiced_bt_cfg_dual_mode.c \
            bt_rfcomm_server.c \
            wiced_bt_sdp_db.c

$(NAME)_INCLUDES := .

BT_CONFIG_DCT_H  := bt_config_dct.h

GLOBAL_DEFINES     += WICED_DCT_INCLUDE_BT_CONFIG

$(NAME)_COMPONENTS := libraries/drivers/bluetooth

VALID_PLATFORMS += BCM9WCDPLUS114 \
                   BCM943909WCD* \
                   BCM943341WCD1 \
                   BCM9WCD1AUDIO \
                   BCM943438WLPTH_2 \
                   BCM943438WCD1 \
                   BCM94343WWCD1 \
                   RB_DUO \
                   RB_DUO_ext
