
# WICED Firmware Management with DFU

This guide provides information for loading WICED firmware using the bootloader DFU mode through the onboard USB.


## Prerequisites

Follow [this](https://github.com/redbear/Duo/blob/master/docs/dfu-util_installation_guide.md) to install the 'dfu-util' tool.


## Updating Firmware

To load application firmware (e.g. the RGB example):

	`$ dfu-util -d 2b04:d058 -a 0 -s 0x08010000 -D redbear.rgb-RB_DUO.bin`

You will see the RGB changes color alternatively, also press the 'SETUP' button to see the effect.

If you are going to deploy the application that depends on the DCT, e.g., the application requiring WiFi configurations, you MUST exactly follow the steps below:

1. Deploy the `DCT.bin_PAD.bin` under the build folder, e.g.: `$ dfu-util -d 2b04:d058 -a 0 -s 0x08008000 -D DCT.bin_PAD.bin`
2. Perform a hard reset.
3. Deploy the application binary, e.g.: `$ dfu-util -d 2b04:d058 -a 0 -s 0x08010000 -D snip.apsta-RB_DUO.bin`

Without the step 2, after you deploying the application and reset, the bootloader won't jump to run the application. It is because that only if the bootloader right enters the DFU mode and the first image to be deployed is of WICED application, the bootloader will set a flag to indicate to run the WICED application on next reboot.

