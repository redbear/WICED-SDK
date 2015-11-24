
## Red Bear Duo Kit (an IoT development board)

The Duo Kit has two boards, the Duo and the RBLink. 

*** There is an issue with OSX 10.11, the RBLink will not work (with OpenOCD/libusb). As a workaround, please use MSD (drag & drop) method, OSX 10.10 or Windows at this moment.


## Red Bear Duo

The Red Bear Duo contains a ST32F205 MCU, AP6212A WiFi (802.11 b/g/n 2.4GHz) + Bluetooth (4.1) combo module and an onboard 2 MB SPI Flash. The MCU has 1 MB Flash and 128 KB SRAM, running at 120 MHz. The wireless module built on Broadcom BCM43438 A1 chip.

![image](RBDuo.png)


## Unlock the Duo

The Duo comes with preloaded firmware that protects some sectors of the Flash area, use the unlock firmware (locate inside the 'test' folder) to unlock it first before uploading any WICED firmware. Use the RBLink and drag & drop the bin to the disk drive named "RBLink".


## WICED SDK

To use the board with WICED SDK 3.3.1:

1. Download Broadcom WICED-SDK-3.3.1 from:

   - http://community.broadcom.com/docs/DOC-2312

2. Unzip WICED-SDK-3.3.1 to your local file-system

3. Download and copy the files in this add-on package to the WICED-SDK-3.3.1 and unzip and replace the original files


## RedBear RBLink (Programmer/Debugger) 

![image](RBLink.png)

The RBLink is a MCU flash programmer/debugger for the Duo to download firmware (also works for other ST MCUs) based on STLink:

1. For Windows, go to ST official website to download and install the STLink driver:

   - http://www.st.com/web/en/catalog/tools/PF260219

* No driver is required for OSX and Linux

2. It also supports USB CDC for debugging using serial print out.


## Compilation

To test WiFi, you can use this command, it is an AP scanner demo:

"make snip.scan-RB_DUO_ES2 JTAG=RBLINK download run"

To test Blutooth LE, you can use this command, it is a proximity reporter demo:

"make snip.bluetooth.ble_proximity_reporter-RB_Duo_ES2 JTAG=RBLINK download run"


## Upload Firmware

1. Stack the Duo on top of the RBLink

2. Connect your RBLink USB to your PC using a micro USB cable

3. Start the Terminal (OSX) or command prompt (Windows) and change the working directory to WICED-SDK-3.3.1>

4. Type in "make rbl.rgb-RB_Duo JTAG=RBLINK download run" and press the Enter on the keyboard to begin build the project that under the folder "WICED-SDK-3.3.1\apps\rbl\rgb". More details about make usage please refer to the Makefile which under "WICED-SDK-3.3.1\"

* For OSX, use "./" before "make", e.g. "./make"

* Remember to short the JTMS D7 and JTCK D6 jumpers (as the photo shows) before burning firmware to the board.


## Drag-N-Drop Programming

The RBLink supports drag and drop programming, so you can easily deploy your firmware.

Using this command to compile the example "make rbl.rgb-RB_Duo", it will create a file with ending "_MSD.bin" in the "build" folder, drag and drop it to the MSD drive via the RBLink, the firmware will be uploaded to the Duo.


## Other Tools

The RBLink is compatible with the STLink Utility
- http://www.st.com/web/en/catalog/tools/PF258168

ST Visual Programmer (STVP)

qstlink

pystlimk

