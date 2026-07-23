#include <Arduino.h>
#include <OneButton.h>
#include "config.h"
#include "app/serviceCLI.hpp"

ServiceCLI serviceCLI;
OneButton bootButton(PIN_BOOT_BUTTON, true);
bool serviceMode = false;

void handleBootButtonClick();

void setup()
{
  Serial.begin(115200);

  pinMode(PIN_ONBOARD_LED, OUTPUT);
  bootButton.attachClick(handleBootButtonClick);

  serviceCLI.init();
}

void loop()
{
  bootButton.tick();

  if(serviceMode)
  {
    serviceCLI.tick();
  }
}

void handleBootButtonClick()
{
	// toggel service mode and signal an active service mode by activating the onboard led
	serviceMode = !serviceMode;
	if(serviceMode)
	{
		serviceCLI.activate();
		digitalWrite(PIN_ONBOARD_LED, HIGH);
	}
	else
	{
		serviceCLI.deactivate();
		digitalWrite(PIN_ONBOARD_LED, LOW);
	}
}