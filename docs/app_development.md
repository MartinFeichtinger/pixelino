# App Development Guid for the Pixelino <!-- omit from toc -->

This is a detailed summary for the app and game development on the pixelino

Table of Contents  
- [IApplication - the blueprint](#iapplication---the-blueprint)
- [AppRegistry - add an app to the main menu](#appregistry---add-an-app-to-the-main-menu)


## IApplication - the blueprint
Each application needs to be a class that inherents from the IApplication interface.
The IApplication interface defines the core functions which each application needs to implemnt.

```c++
void MyGame::onStart() {
    // fancy intro animation
    // pre game start settings (level, difficulty)
    // loading previous saved games (not supported yet)
}

void MyGame::onStop() {
    // fancy outro animation
    // save games (not supported yet)
}

void MyGame::tick(uint32_t time_ms) {
    // implent your game logic here
    // the time in milliseconds since bootup is provided for time dependet logic
}

void MyGame::draw() {
    // implement here your game graphics rendering
    // this function gets called periodicly with 30 FPS
}

void MyGame::onButtonEvent(ButtonId id, ButtonEvent event) {
    // implement here the button input depending logic
    // this function gets called when a button event gets triggerd
}
```

Furthermore the IApplication interface simplifies the complexe programm structure in the background. Therfore all provided driver and types are accesible in an simpler programm language.

## AppRegistry - add an app to the main menu
Apps use self-registration so they can appear in the main menu without needing to modify main_menu.cpp or include app header files across the project. This relies on an Immediately Invoked Lambda Expression (IIFE) assigned to a static variable, which runs automatically at system startup. Each registration requires three elements.
* **Name**: The name of the application.
* **Icon**: Pointer to the pixel data rendered on the display.
* **Factory Function**: A tiny lambda ([]() { return new MyGame(); }) that acts as a creation recipe. This lets the menu instantiate your application on demand when selected, keeping memory clean until the game is actually played.

```c++
static bool isMyGameRegistered = []() {
    // custom icon shown in the main menu
    static const core::Color iconPixels[core::config::display::num_leds] = {
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
        // ... (remaining pixel data)
    };

    app::AppRegistry::getInstance().registerApp(
        "MyGame",
        iconPixels,
        // factory function: tells the registry how to create the app when launched
        []() -> app::IApplication* {
            return new MyGame();
        }
    );
    return true;
}();
```