#include <Arduino.h>
#include <OneButton.h>

#include "config.h"
#include "app/serviceCLI.h"

OneButton bootButton(PIN_BOOT_BUTTON, true);
bool serviceMode = false;

void handleBootButtonClick();

void setup()
{
  Serial.begin(115200);

  pinMode(PIN_ONBOARD_LED, OUTPUT);
  bootButton.attachClick(handleBootButtonClick);

  SERVICE_CLI_init();
}

void loop()
{
  bootButton.tick();

  if(serviceMode)
  {
    SERVICE_CLI_runService();
  }
}

void handleBootButtonClick()
{
	// toggel service mode and signal an active service mode by activating the onboard led
	serviceMode = !serviceMode;
	if(serviceMode)
	{
		SERVICE_CLI_activate();
		digitalWrite(PIN_ONBOARD_LED, HIGH);
	}
	else
	{
		SERVICE_CLI_deactivate();
		digitalWrite(PIN_ONBOARD_LED, LOW);
	}
}