
# WICED Firmware Compilation Guide

This guide provides information for compiling WICED application firmware and loading firmware using the bootloader DFU mode through the onboard USB or the RBLink.


## Firmware Compilation

Start a command line tool, e.g. Command Prompt (Windows) or Terminal (OSX and Linux). Navigate to the WICED SDK root folder.

The following commands use the snip.scan project as an example.

For OSX & Linux:
	
	$ ./make redbear.rgb-RB_DUO

For Windows:
	
	C:\WICED-Studio-4.1\43xxx_Wi-Fi> make redbear.rgb-RB_DUO

After that, in the 'build/redbear_rgb-RB_DUO/binary' folder, there are two files compiled 'redbear_rgb-RB_DUO.bin' and 'redbear_rgb-RB_DUO.bin_MSD.bin'. The first one is for uploading using DFU mode while the second one is for using the RBLink drag and drop method.


## Uploading Firmware

There are three methods for uploading WICED application firmware for the Duo:

* Bootloader DFU Mode
* RBLink
	* OpenOCD (official WICED SDK method)
	* Drag & Drop


## Bootloader DFU Mode

* see [this](../firmware/DFU/README.md) document for details.


## RBLink

* See [this](../firmware/RBLink/README.md) document for details.


