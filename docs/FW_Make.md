
# WICED Firmware Compilation Guide


To test WiFi, you can use this command, it is an AP scanner demo:

"make snip.scan-RB_DUO JTAG=RBLINK download run" - WiFi firmware in internal flash    
or   
"make snip.scan-RB_DUO_ext JTAG=RBLINK download download_apps run" - WiFi firmware in external serial flash

To test Blutooth LE, you can use this command, it is a proximity reporter demo:

"make snip.bluetooth.ble_proximity_reporter-RB_DUO JTAG=RBLINK download run"   
or    
"make snip.bluetooth.ble_proximity_reporter-RB_DUO_ext JTAG=RBLINK download download_apps run"

Unlike the default WICED SDK, there are two methods for uploading WICED firmware, read the following documents:

* Bootloader DFU mode
* RBLink

