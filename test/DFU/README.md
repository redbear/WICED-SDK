
# WICED Firmware Management With DFU

This guide provides information for loading WICED firmware using the bootloader DFU mode through the onboard USB.

Currectly, it supports the RedBear Duo IoT development board.


## Prerequisites

Follow [this](https://github.com/redbear/Duo/blob/master/docs/dfu.md) to install the 'dfu-util' tool.


## Updating Firmware

You need to update the DCT that the SDK compiled first:

	$ dfu-util -d 2b04:d058 -a 0 -s 0x08004000 -D DCT.bin

After that, you can load the main firmware:

	$ dfu-util -d 2b04:d058 -a 0 -s 0x0800C000 -D rgb.bin

You will see the RGB changes color alternatively, also press the 'SETUP' button to see the effect.

