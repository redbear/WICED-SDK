#
# Copyright 2016, RedBear
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of RedBear;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of RedBear Corporation.
#

NAME := Platform_RB_DUO

WLAN_CHIP            := 43438
WLAN_CHIP_REVISION   := A1
WLAN_CHIP_FAMILY     := 4343x
HOST_MCU_FAMILY      := STM32F2xx
HOST_MCU_VARIANT     := STM32F2x5
HOST_MCU_PART_NUMBER := STM32F205RGT6

PLATFORM_SUPPORTS_BUTTONS := 1

# BCM943438WCD1
BT_CHIP          := 43438
BT_CHIP_REVISION := A1
BT_MODE          ?= HCI
BT_CHIP_XTAL_FREQUENCY := 26MHz

# enable BT application for BCMUSI22 platfrom
ifneq ($(filter BCM9$(WLAN_CHIP)%,$(VALID_PLATFORMS)),)
$(eval VALID_PLATFORMS += $(PLATFORM))
endif # check valid platforms


VALID_BUSES := SDIO SPI

ifndef BUS
BUS := SDIO
endif

EXTRA_TARGET_MAKEFILES +=  $(MAKEFILES_PATH)/standard_platform_targets.mk

ifeq ($(BUS),SDIO)
INTERNAL_MEMORY_RESOURCES = $(ALL_RESOURCES)
GLOBAL_DEFINES           += WWD_DIRECT_RESOURCES
endif


# Global includes
GLOBAL_INCLUDES  := .
GLOBAL_INCLUDES  += $(WICED_BASE)/libraries/inputs/gpio_button

# Global defines
# HSE_VALUE = STM32 crystal frequency = 26MHz (needed to make UART work correctly)
GLOBAL_DEFINES += HSE_VALUE=26000000
GLOBAL_DEFINES += $$(if $$(NO_CRLF_STDIO_REPLACEMENT),,CRLF_STDIO_REPLACEMENT)

# Components
$(NAME)_COMPONENTS += drivers/spi_flash \
                      inputs/gpio_button

# Source files
$(NAME)_SOURCES := platform.c


# WICED APPS
# APP0 and FILESYSTEM_IMAGE are reserved main app and resources file system
# FR_APP := resources/sflash/snip_ota_fr-WSDB734AP6212.stripped.elf
# DCT_IMAGE :=
# OTA_APP :=
# FILESYSTEM_IMAGE :=
# WIFI_FIRMWARE :=
# APP0 :=
# APP1 :=
# APP2 :=

# WICED APPS LOOKUP TABLE, skip the first 4K since the dfu-util can not access the SPI flash from address 0x0
APPS_LUT_HEADER_LOC := 0x1000 
APPS_START_SECTOR := 2

ifneq ($(APP),bootloader)
ifneq ($(MAIN_COMPONENT_PROCESSING),1)
$(info +-----------------------------------------------------------------------------------------------------+ )
$(info | IMPORTANT NOTES                                                                                     | )
$(info +-----------------------------------------------------------------------------------------------------+ )
$(info | Wi-Fi MAC Address                                                                                   | )
$(info |    The target Wi-Fi MAC address is defined in <WICED-SDK>/generated_mac_address.txt                 | )
$(info |    Ensure each target device has a unique address.                                                  | )
$(info +-----------------------------------------------------------------------------------------------------+ )
$(info | MCU & Wi-Fi Power Save                                                                              | )
$(info |    It is *critical* that applications using WICED Powersave API functions connect an accurate 32kHz | )
$(info |    reference clock to the sleep clock input pin of the WLAN chip. Please read the WICED Powersave   | )
$(info |    Application Note located in the documentation directory if you plan to use powersave features.   | )
$(info +-----------------------------------------------------------------------------------------------------+ )
endif
endif
