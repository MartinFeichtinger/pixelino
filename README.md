# Pixelino
Pixelino is a project for a soldering workshop for children. The core idea is to build a small games console based on an ESP32, featuring an 8×8 RGB LED display, basic input buttons and an audio modul. The software aritectur is based on the Arduino framework and further open source Arduino librarys listed below.

* **FastLED** - display and colors
* **OneButton** - button event generation
* **SimpleCLI** - controling and debugging

## Set up development system (recommented way)
1. Install Visual Studio Code
2. Install the gcc compiler
3. Install the extensions C/C++ and PlatformIO IDE
4. Clone the pixelino github repo (directly in vs code or extern and open the hole project folder with vs code) 
5. Click compile (at the first compilation depentend libraies will be downloade)
6. Connect the ESP and upload the firmware
7. *Now everything should work...*
8. For serial communication and debugging open the serial monitor and click on the BOOT button (accesible via a hole on the backside of the pixelino) to activate the service CLI.
9.  Create a new branche for your own first game