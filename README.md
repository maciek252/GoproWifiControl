# GoproWifiControl

Tested on https://en.wikipedia.org/wiki/NodeMCU (v0.9) and Gopro 4 Black.

Install the NodeMcu toolchain for Arduino as described here:

http://www.instructables.com/id/Quick-Start-to-Nodemcu-ESP8266-on-Arduino-IDE/

Set the ssid and password in the cam and put the corresponding values in the code (ssid and password vars) and flash the code into the board (connected with USB). Turn wifi on in the cam (the blue LED blinks every few seconds and the wifi symbol is on). Put the cam into the photo mode (a photo camera symbol is shown in the display).

Connect the IR port from STorM32 to nodes GND and D5. Add a resistor and/or a LED between these wires, so that there are no unstable states. I have 17 kohms, as far as I can decode the colors (red, purple, orange, gold). You can also test by connecting D5 between 3.3V (shots photos) and GND.

Currently it takes two photos for every IR signal, one with EV=0 and one with EV=-1.0. The red LED on cam flashes when taking a photo.

Based on https://github.com/KonradIT/goprowifihack/blob/master/HERO4/WifiCommands.md

