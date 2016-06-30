#
# Copyright 2015, Broadcom Corporation
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom Corporation.
#

NAME := Bluetooth_Low_Energy_Ble_Central_demo_Application

$(NAME)_SOURCES    := ble_central_multilink.c 	\
                      wiced_bt_cfg.c		\
                      ble_peripheral_nano.c \
                      cjson.c \
                      aws_config.c \
                   		aws_common.c

GLOBAL_DEFINES     += APPLICATION_STACK_SIZE=4096
GLOBAL_DEFINES     += HARDWARE_IO_WORKER_THREAD_STACK_SIZE=1024   
GLOBAL_DEFINES     += NETWORKING_WORKER_THREAD_STACK_SIZE=1024            

$(NAME)_INCLUDES   := .

WIFI_CONFIG_DCT_H  := wifi_config_dct.h

$(NAME)_COMPONENTS := libraries/drivers/bluetooth_le \
					  protocols/MQTT \
                      inputs/gpio_button \
                      libraries/utilities/JSON_parser
$(NAME)_COMPONENTS += libraries/daemons/HTTP_server 
$(NAME)_COMPONENTS += libraries/utilities/JSON_parser
$(NAME)_COMPONENTS += daemons/HTTP_server \
                      daemons/DNS_redirect \
                      protocols/DNS

APPLICATION_DCT := aws_config_dct.c

#GLOBAL_DEFINES     += USE_HTTPS
ifneq (,$(findstring USE_HTTPS,$(GLOBAL_DEFINES)))
CERTIFICATE := $(SOURCE_ROOT)resources/certificates/brcm_demo_server_cert.cer
PRIVATE_KEY := $(SOURCE_ROOT)resources/certificates/brcm_demo_server_cert_key.key
endif

$(NAME)_RESOURCES  := apps/aws_iot/rootca.cer

$(NAME)_RESOURCES += images/brcmlogo.png \
                     images/brcmlogo_line.png \
                     images/favicon.ico \
                     images/scan_icon.png \
                     images/wps_icon.png \
                     images/64_0bars.png \
                     images/64_1bars.png \
                     images/64_2bars.png \
                     images/64_3bars.png \
                     images/64_4bars.png \
                     images/64_5bars.png \
                     images/tick.png \
                     images/cross.png \
                     images/lock.png \
                     images/progress.gif \
                     scripts/general_ajax_script.js \
                     scripts/wpad.dat \
                     apps/aws_iot/aws_config.html \
                     config/scan_page_outer.html \
                     styles/buttons.css \
                     styles/border_radius.htc
                      
VALID_PLATFORMS += BCM9WCDPLUS114 \
                   BCM943909WCD1 \
                   BCM943909WCD1_3 \
                   BCM943341WCD1 \
                   BCM9WCD1AUDIO \
                   BCM943438WLPTH_2 \
                   RBL205IOT \
                   RB_DUO
