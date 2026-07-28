#include "core/config.hpp"
#include "driver/display.hpp"
#include "driver/button_manager.hpp"
#include "app/serviceCLI.hpp"
#include <OneButton.h>
#include <Arduino.h>

pixelino::driver::ButtonManager buttonManager;

void setup() {
    Serial.begin(115200);

	// hardware inits
	pixelino::driver::Display::getInstance().begin();
	buttonManager.begin();

	// service inits
	pixelino::app::ServiceCLI::getInstance().begin(buttonManager);
}

void loop() {
	buttonManager.tick();
    pixelino::app::ServiceCLI::getInstance().tick(); 
}