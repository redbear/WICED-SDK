#
# Copyright 2015, RedBear
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of RedBear;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom Corporation.
#

NAME := Platform_RB_DUO

WLAN_CHIP            := 43438
WLAN_CHIP_REVISION   := A1
HOST_MCU_FAMILY      := STM32F2xx
HOST_MCU_VARIANT     := STM32F2x5
HOST_MCU_PART_NUMBER := STM32F205RGT6
BT_CHIP              := 43438
BT_CHIP_REVISION     := A1
BT_MODE              ?= HCI


WIFI_FIRMWARE_IN_SPI_FLASH = NO
#if YES, USES_RESOURCE_FILESYSTEM also must be define in platform_config.h

ifneq ($(WIFI_FIRMWARE_IN_SPI_FLASH),YES)
INTERNAL_MEMORY_RESOURCES = $(ALL_RESOURCES)
endif

EXTRA_TARGET_MAKEFILES +=  $(MAKEFILES_PATH)/standard_platform_targets.mk

ifndef BUS
BUS := SDIO
endif

VALID_BUSES := SDIO SPI

ifeq ($(BUS),SDIO)
WIFI_IMAGE_DOWNLOAD := direct
ifneq ($(WIFI_FIRMWARE_IN_SPI_FLASH),YES)
GLOBAL_DEFINES      += WWD_DIRECT_RESOURCES
endif
else
ifeq ($(BUS),SPI)
WIFI_IMAGE_DOWNLOAD := buffered
endif
endif

# Global includes
GLOBAL_INCLUDES += .

# Global defines
# HSE_VALUE = STM32 crystal frequency = 26MHz (needed to make UART work correctly)
GLOBAL_DEFINES += HSE_VALUE=26000000
GLOBAL_DEFINES += $$(if $$(NO_CRLF_STDIO_REPLACEMENT),,CRLF_STDIO_REPLACEMENT)

# Components
$(NAME)_COMPONENTS += drivers/spi_flash

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

# WICED APPS LOOKUP TABLE
APPS_LUT_HEADER_LOC := 0x0000
APPS_START_SECTOR := 1

ifeq ($(WIFI_FIRMWARE_IN_SPI_FLASH),YES)
ifneq ($(APP),bootloader)
FILESYSTEM_IMAGE = $(OUTPUT_DIR)/resources/filesystem.bin
endif
endif

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
