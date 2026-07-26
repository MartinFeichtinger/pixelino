#include <Arduino.h>
#include <OneButton.h>
#include "core/config.hpp"
#include "driver/display.hpp"
#include "app/serviceCLI.hpp"

OneButton bootButton(pixelino::core::config::pin::boot_button, true);

void handleBootButtonClick() {
	// activate/deactivate the service mode
	pixelino::app::ServiceCLI::getInstance().toggle();
}

void setup() {
    Serial.begin(115200);

	// hardware inits
	pixelino::driver::Display::getInstance().begin();

	// service inits
	pixelino::app::ServiceCLI::getInstance().begin();
	bootButton.attachClick(handleBootButtonClick);
}

void loop() {
    bootButton.tick();
    pixelino::app::ServiceCLI::getInstance().tick(); 
}