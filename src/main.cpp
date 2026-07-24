#include <Arduino.h>
#include <OneButton.h>
#include "config.hpp"
#include "app/serviceCLI.hpp"

OneButton bootButton(config::pin::boot_button, true);

void handleBootButtonClick() {
	// activate/deactivate the service mode
	ServiceCLI::getInstance().toggle();
}

void setup() {
    Serial.begin(115200);

	ServiceCLI::getInstance().begin();
    bootButton.attachClick(handleBootButtonClick);
}

void loop() {
    bootButton.tick();
    ServiceCLI::getInstance().tick(); 
}