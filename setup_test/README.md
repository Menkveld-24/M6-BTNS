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
 Then select the appropriate serial port and then you can try to upload the setup_test.ino sketch. If it works you should see a blue led turning on and off twice a second. The serial monitor can be opened at 115200.