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

## How to Start Programming Games

Every game inherently follows the same blueprint and must implement a standard set of core components.

**Prerequisites & Reference Code**
* **[App Development Documentation](./docs/app_development.md):** Review this first to understand core architectural concepts and available framework APIs.
* **[Paint Reference Game](./src/apps/paint/paint.hpp):** Inspect this simple implementation for practical examples of display rendering, button inputs, and timing logic. Use it as a starter template if your game shares similar mechanics.
* **[Empty Template](./src/apps/empty_template/empty_template.hpp):** Use this minimal boilerplate setup if you prefer building your game from scratch.

**Setup Steps**
1. **Create a Git branch:** Create a new branch named after your game.
2. **Create the app folder:** Add a new directory under `src/apps/<your_game_name>/`.
3. **Copy boilerplate files:** Copy the `.hpp` and `.cpp` template files into your new folder and rename them to match your game.
4. **Implement your game:** Add your custom logic, and start coding.