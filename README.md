# macs_firmware
Machine Access Control - Firmware for RF8266

Using Arduino IDE
=================

Requires the esp8266 package, add http://arduino.esp8266.com/stable/package_esp8266com_index.json to the Additional Baords Manager URL.

Once that is installed also need to add the SeeedRFID library from here: https://github.com/Seeed-Studio/RFID_Library

Also need to have a USB->serial cable to connect to the console header on the Huzzah board.

To build, copy configure.h.dist to configure.h and set values for SSID, PASSWORD, and HOSTADDRESS.


