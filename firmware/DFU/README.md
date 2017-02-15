
# WICED Firmware Management with DFU

This guide provides information for loading WICED firmware using the bootloader DFU mode through the onboard USB.


## Prerequisites

Follow [this](https://github.com/redbear/Duo/blob/master/docs/dfu-util_installation_guide.md) to install the 'dfu-util' tool.


## Updating Firmware

To load application firmware (e.g. the RGB example):

	$ dfu-util -d 2b04:d058 -a 0 -s 0x08010000 -D redbear.rgb-RB_DUO.bin

You will see the RGB changes color alternatively, also press the 'SETUP' button to see the effect.

