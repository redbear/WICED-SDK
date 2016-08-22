
# WICED Firmware Management with RBLink

This guide provides information for loading WICED firmware using the RBLink.

Use the RBLink, you can upload firmware easily. Also, you can use this method to deploy your firmware to other people without providing the source code.

Note: Use RBLink to upload firmware will clear the default bootloader, if you need to reload the default bootloader using RBLink drag and drop method.

Get the default bootloader from [this](https://github.com/redbear/Duo/tree/master/firmware/bootloader) URL.

## Unlock the Duo

The preloaded firmware is protected by default, you need to unlock it before using the RBLink to upload firmware (only need to do once).

* Stack the Duo on top of the RBLink

* Connect your RBLink USB to your PC using a micro USB cable

	* Note: Remember to short the JTMS D7 and JTCK D6 jumpers before burning firmware to the board.

* You will see there is a drive named 'RBLink' in your computer.

* Inside this folder, drag and drop the 'RBDuo_Unlock.bin' file to the RBLink drive, then the onboard RGB LED will flash green color very fast.

* Disconnect it from your computer USB port.

## RBLink with OpenOCD

* Connect the RBLink to your computer USB port again.

* To test the onboard RGB LED:

		$ ./make redbear.rgb-RB_DUO JTAG=RBLINK download run

	You will see the RGB changes color alternatively, also press the 'SETUP' button to see the effect.

* To test WiFi, you can use this command, it is an AP scanner demo:

		$ ./make snip.scan-RB_DUO JTAG=RBLINK download run    

	Use serial port tool (e.g. screen or PuTTY) to see the output.
	
* To test Blutooth LE, you can use this command, it is a proximity reporter demo:

		$ ./make snip.bluetooth.ble_proximity_reporter-RB_DUO JTAG=RBLINK download run  
	
	Use iOS 'LightBlue' App or Android 'B-BLE' App to see.

## RBLink with Drag & Drop

* Connect the RBLink to your computer USB port again.

* You will see the 'RBLink' drive again in your computer.

* Drag & drop the 'redbear.rgb-RB_DUO.bin_MSD.bin' to the drive. 

* You will see the RGB changes color alternatively, also press the 'SETUP' button to see the effect.
