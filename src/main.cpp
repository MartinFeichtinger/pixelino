#include "app/app_manager.hpp"
#include <Arduino.h>

void setup() {
	pixelino::app::AppManager::getInstance().begin();
}

void loop() {
	pixelino::app::AppManager::getInstance().tick();
}