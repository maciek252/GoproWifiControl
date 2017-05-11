# GoproWifiControl

Tested on https://en.wikipedia.org/wiki/NodeMCU and Gopro 4 Black.

Install the NodeMcu toolchain for Arduino as described here:

http://www.instructables.com/id/Quick-Start-to-Nodemcu-ESP8266-on-Arduino-IDE/

Set the ssid and password in the cam and put the corresponding values in the code (ssid and password vars).

Connect the IR port from STorM32 to nodes GND and D5. Add a resistor and/or a LED between these wires, so that there are no unstable states. I have 17 kohms, as far as I can decode the colors (red, violet, orange, gold).

Currently it takes two photos for every IR signal, one with EV=0 and one with EV=-1.0.

Based on https://github.com/KonradIT/goprowifihack/blob/master/HERO4/WifiCommands.md

