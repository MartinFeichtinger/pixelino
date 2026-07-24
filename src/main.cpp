#include <Arduino.h>
#include <OneButton.h>
#include "config.hpp"
#include "types.hpp"
#include "app/serviceCLI.hpp"
#include "driver/display.hpp"

ServiceCLI serviceCLI;
OneButton bootButton(config::pin::boot_button, true);
bool serviceMode = false;

void handleBootButtonClick();

void setup()
{
  Serial.begin(115200);

  pinMode(config::pin::onboard_led, OUTPUT);
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
		digitalWrite(config::pin::onboard_led, HIGH);
	}
	else
	{
		serviceCLI.deactivate();
		digitalWrite(config::pin::onboard_led, LOW);
	}
}