# Pixelino
Pixelino is a project for a soldering workshop for children. The core idea is to build a small games console based on an ESP32, featuring an 8×8 RGB LED display, basic input buttons and an audio modul. The software aritectur is base on the Arduino framework and further open source Arduino librarys listed below.

* **FastLED** - display and colors
* **OneButton** - button event generation
* **SimpleCLI** - controling and debugging

## Getting Started (recommented way)
1. Install Visual Studio Code
2. Install the extensions C/C++ and PlatformIO IDE
3. Clone the pixelino github repo (directly in vs code or extern and open the hole project folder with vs code) 
4. Click compile (at the first compilation depentend libraies will be downloade)
5. Connect the ESP and upload the firmware
6. *Now everything should work...*
7. For serial communication and debugging open the serial monitor and click on the onboard button to activate ther service CLI.
8. Look into src/apps/paint to see how to programm an appication for the pixelino
