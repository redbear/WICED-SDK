
The Particle firmware protects the flash by default. This is to unlock the Duo using OpenOCD, remember to change the system path (OSX/Linux32/Linux64/Win32) in the following sample command to your one.

Change the current working folder to tools first and then run this command:

$ ./openocd/OSX/openocd-all-brcm-libftdi -d1 -s "./openocd" -f "./openocd/RBLINK.cfg" -c "init; reset halt; Unlock (); reset halt; program ../firmware/RBLink/redbear.rgb-RB_DUO.bin_MSD.bin verify 0x8000000; reset run; exit"

This will unlock the Duo and load the RGB demo firmware, next time, you can use it to develop your own WICED application without to unlock it again.
