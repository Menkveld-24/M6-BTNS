# Installation
I installed this project on macos so things can be slightly different on windows.

Make sure to install the cp2012 driver
````
https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
````

The arduino IDE is fully usable for our esp8266.
Open arduino->preferences and you should see a text field "Board manager URL's", paste this link in: 
````
https://arduino.esp8266.com/stable/package_esp8266com_index.json
````

Then close the setting panel and install the esp8266 package:
Tools->try to select a board and pick "Board manager", search for esp8266 and install the package.

Plug in the esp8266 (make sure the cable has data lines) and at tools->board select the "NodeMCU 1.0 (ESP-12E Module)".
There now should be a ton more options at the tools but i havent touched any.

Compile and upload the sketch, if the device needs to be a master short pin D1 to ground. Else leave it open to be a slave. A master currently supports 10 slaves (code limitation atm)

For simultaneous serial monitoring dont use the arduino monitor. 
Open the terminal and run:
````
screen [port] [baud]
````
This allows you to open up multiple serial monitors at the same time (in different terminal windows ofc)
The baud rate is set to 115200 for the project