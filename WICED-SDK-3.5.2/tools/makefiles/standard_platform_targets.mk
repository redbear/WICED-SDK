#
# Copyright 2015, Broadcom Corporation
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom Corporation.
#

.PHONY: bootloader download_bootloader no_dct download_dct download

BOOTLOADER_TARGET := waf.bootloader-NoOS-$(PLATFORM)
BOOTLOADER_OUTFILE := $(BUILD_DIR)/$(BOOTLOADER_TARGET)/binary/$(BOOTLOADER_TARGET)
BOOTLOADER_LOG_FILE ?= $(BUILD_DIR)/bootloader.log
GENERATED_MAC_FILE := $(SOURCE_ROOT)generated_mac_address.txt
MAC_GENERATOR      := $(TOOLS_ROOT)/mac_generator/mac_generator.pl

#APPS LOOK UP TABLE PARAMS
APPS_LUT_DOWNLOAD_DEP :=

#SFLASH_WRITER_APP Required by  download_apps
SFLASH_APP_PLATFROM_BUS := $(PLATFORM)-$(BUS)
SFLASH_APP_BCM4390 := 0

ifeq (,$(and $(OPENOCD_PATH),$(OPENOCD_FULL_NAME)))
	$(error Path to OpenOCD has not been set using OPENOCD_PATH and OPENOCD_FULL_NAME)
endif


ifneq ($(VERBOSE),1)
BOOTLOADER_REDIRECT	= > $(BOOTLOADER_LOG_FILE)
endif

ifneq (,$(findstring waf/bootloader, $(BUILD_STRING))$(findstring wwd/, $(BUILD_STRING)))
NO_BUILD_BOOTLOADER:=1
NO_BOOTLOADER_REQUIRED:=1
endif

#$(info standard_platform_targets.mk ota2_support=$(OTA2_SUPPORT))
ifeq (1,$(OTA2_SUPPORT))
NO_BUILD_BOOTLOADER:=1
NO_BOOTLOADER_REQUIRED:=1
endif

ifneq ($(NO_BUILD_BOOTLOADER),1)
bootloader:
	$(QUIET)$(ECHO) Building Bootloader
	$(QUIET)$(MAKE) -r -f $(SOURCE_ROOT)Makefile $(BOOTLOADER_TARGET) -I$(OUTPUT_DIR)  SFLASH= EXTERNAL_WICED_GLOBAL_DEFINES=$(EXTERNAL_WICED_GLOBAL_DEFINES) SUB_BUILD=bootloader $(BOOTLOADER_REDIRECT)
	$(QUIET)$(ECHO) Finished Building Bootloader
	$(QUIET)$(ECHO_BLANK_LINE)

download_bootloader: bootloader display_map_summary
	$(QUIET)$(ECHO) Downloading Bootloader ...
	$(QUIET)$(call CONV_SLASHES,$(OPENOCD_FULL_NAME)) -f $(OPENOCD_PATH)$(JTAG).cfg -f $(OPENOCD_PATH)$(HOST_OPENOCD).cfg -f $(OPENOCD_PATH)$(HOST_OPENOCD)-flash-app.cfg -c "verify_image_checksum $(BOOTLOADER_OUTFILE).stripped.elf" -c shutdown $(DOWNLOAD_LOG) 2>&1 && $(ECHO) No changes detected && $(ECHO_BLANK_LINE) || $(call CONV_SLASHES,$(OPENOCD_FULL_NAME)) -f $(OPENOCD_PATH)$(JTAG).cfg -f $(OPENOCD_PATH)$(HOST_OPENOCD).cfg -f $(OPENOCD_PATH)$(HOST_OPENOCD)-flash-app.cfg -c "flash write_image erase $(BOOTLOADER_OUTFILE).stripped.elf" -c shutdown $(DOWNLOAD_LOG) 2>&1 && $(ECHO) Download complete && $(ECHO_BLANK_LINE) || $(ECHO) "**** OpenOCD failed - ensure you have installed the driver from the drivers directory, and that the debugger is not running **** In Linux this may be due to USB access permissions. In a virtual machine it may be due to USB passthrough settings. Check in the task list that another OpenOCD process is not running. Check that you have the correct target and JTAG device plugged in. ****"

copy_bootloader_output_for_eclipse: build_done
	$(QUIET)$(call MKDIR, $(BUILD_DIR)/eclipse_debug/)
	$(QUIET)$(CP) $(BOOTLOADER_OUTFILE).elf $(BUILD_DIR)/eclipse_debug/last_bootloader.elf


else
ifeq (1,$(NO_BOOTLOADER_REQUIRED))
bootloader:
	@:

else
bootloader:
	$(QUIET)$(ECHO) Skipping building bootloader due to commandline spec

endif

download_bootloader:
	$(QUIET)$(ECHO) Skipping building bootloader due to NO_BOOTLOADER_REQUIRED

copy_bootloader_output_for_eclipse:
	@:

endif



ifneq (no_dct,$(findstring no_dct,$(MAKECMDGOALS)))
download_dct: $(STRIPPED_LINK_DCT_FILE) display_map_summary download_bootloader
	$(QUIET)$(ECHO) Downloading DCT ...
	$(QUIET)$(call CONV_SLASHES,$(OPENOCD_FULL_NAME)) -f $(OPENOCD_PATH)$(JTAG).cfg -f $(OPENOCD_PATH)$(HOST_OPENOCD).cfg -f $(OPENOCD_PATH)$(HOST_OPENOCD)-flash-app.cfg -c "verify_image_checksum $(STRIPPED_LINK_DCT_FILE)" -c shutdown $(DOWNLOAD_LOG) 2>&1 && $(ECHO) No changes detected && $(ECHO_BLANK_LINE) || $(call CONV_SLASHES,$(OPENOCD_FULL_NAME)) -f $(OPENOCD_PATH)$(JTAG).cfg -f $(OPENOCD_PATH)$(HOST_OPENOCD).cfg -f $(OPENOCD_PATH)$(HOST_OPENOCD)-flash-app.cfg -c "flash write_image erase $(STRIPPED_LINK_DCT_FILE)" -c shutdown $(DOWNLOAD_LOG) 2>&1 && $(ECHO) Download complete && $(ECHO_BLANK_LINE) || $(ECHO) "**** OpenOCD failed - ensure you have installed the driver from the drivers directory, and that the debugger is not running **** In Linux this may be due to USB access permissions. In a virtual machine it may be due to USB passthrough settings. Check in the task list that another OpenOCD process is not running. Check that you have the correct target and JTAG device plugged in. ****"

else
download_dct:
	@:

no_dct:
	$(QUIET)$(ECHO) DCT unmodified

endif

download: $(STRIPPED_LINK_OUTPUT_FILE) display_map_summary download_bootloader $(if $(findstring no_dct,$(MAKECMDGOALS)),,download_dct)
	$(QUIET)$(ECHO) Downloading Application ...
	$(QUIET)$(call CONV_SLASHES,$(OPENOCD_FULL_NAME)) -f $(OPENOCD_PATH)$(JTAG).cfg -f $(OPENOCD_PATH)$(HOST_OPENOCD).cfg -f $(OPENOCD_PATH)$(HOST_OPENOCD)-flash-app.cfg -c "verify_image_checksum $(STRIPPED_LINK_OUTPUT_FILE)" -c shutdown $(DOWNLOAD_LOG) 2>&1 && $(ECHO) No changes detected && $(ECHO_BLANK_LINE) || $(call CONV_SLASHES,$(OPENOCD_FULL_NAME)) -f $(OPENOCD_PATH)$(JTAG).cfg -f $(OPENOCD_PATH)$(HOST_OPENOCD).cfg -f $(OPENOCD_PATH)$(HOST_OPENOCD)-flash-app.cfg -c "flash write_image erase $(STRIPPED_LINK_OUTPUT_FILE)" -c shutdown $(DOWNLOAD_LOG) 2>&1 && $(ECHO) Download complete && $(ECHO_BLANK_LINE) || $(ECHO) "**** OpenOCD failed - ensure you have installed the driver from the drivers directory, and that the debugger is not running **** In Linux this may be due to USB access permissions. In a virtual machine it may be due to USB passthrough settings. Check in the task list that another OpenOCD process is not running. Check that you have the correct target and JTAG device plugged in. ****"

ifeq (download,$(filter download,$(MAKECMDGOALS)))
APPS_LUT_DOWNLOAD_DEP := download
endif

download_apps: APPS_LUT_DOWNLOAD

run: $(SHOULD_I_WAIT_FOR_DOWNLOAD)
	$(QUIET)$(ECHO) Resetting target
	$(QUIET)$(call CONV_SLASHES,$(OPENOCD_FULL_NAME)) -c "log_output $(OPENOCD_LOG_FILE)" -f $(OPENOCD_PATH)$(JTAG).cfg -f $(OPENOCD_PATH)$(HOST_OPENOCD).cfg -c init -c "reset run" -c shutdown $(DOWNLOAD_LOG) 2>&1 && $(ECHO) Target running


copy_output_for_eclipse: build_done copy_bootloader_output_for_eclipse
	$(QUIET)$(call MKDIR, $(BUILD_DIR)/eclipse_debug/)
	$(QUIET)$(CP) $(LINK_OUTPUT_FILE) $(BUILD_DIR)/eclipse_debug/last_built.elf


combine_bins: build_done
	$(ECHO) Merge all bin files into a single bin for MSD upload
	$(OBJCOPY) -I binary -O binary --pad-to 0x4000 $(BOOTLOADER_OUTFILE).bin $(BOOTLOADER_OUTFILE)_PAD.bin
	$(OBJCOPY) -I binary -O binary --pad-to 0x8000 $(FINAL_DCT_FILE) $(FINAL_DCT_FILE)_PAD.bin
ifeq ($(HOST_OS),Win32)
	copy /b $(subst /,\,$(BOOTLOADER_OUTFILE))_PAD.bin + $(subst /,\,$(FINAL_DCT_FILE))_PAD.bin + $(subst /,\,$(FINAL_OUTPUT_FILE)) $(subst /,\,$(FINAL_OUTPUT_FILE))_MSD.bin
else
	$(CAT) $(BOOTLOADER_OUTFILE)_PAD.bin $(FINAL_DCT_FILE)_PAD.bin $(FINAL_OUTPUT_FILE) > $(FINAL_OUTPUT_FILE)_MSD.bin
endif
	$(ECHO) Location: $(FINAL_OUTPUT_FILE)_MSD.bin


debug: $(BUILD_STRING) $(SHOULD_I_WAIT_FOR_DOWNLOAD)
	$(QUIET)$(GDB_COMMAND) $(LINK_OUTPUT_FILE) -x .gdbinit_attach


$(GENERATED_MAC_FILE): $(MAC_GENERATOR)
	$(QUIET)$(PERL) $<  > $@


EXTRA_PRE_BUILD_TARGETS  += $(GENERATED_MAC_FILE) bootloader
EXTRA_POST_BUILD_TARGETS += copy_output_for_eclipse combine_bins

