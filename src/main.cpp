#include <Arduino.h>
#include <OneButton.h>

#include "config.h"

OneButton bootButton(BOOT_BUTTON, true);
bool serviceMode = false;

void handleBootButtonClick();

void setup()
{
  Serial.begin(115200);
  pinMode(ONBOARD_LED, OUTPUT);
  bootButton.attachClick(handleBootButtonClick);  
}

void loop()
{
  bootButton.tick();

  if(serviceMode)
  {
    Serial.println(millis());
  }
}

void handleBootButtonClick()
{
  // toggel service mode and signal an active service mode by activating the onboard led
  serviceMode = !serviceMode;
  digitalWrite(ONBOARD_LED, serviceMode ? HIGH : LOW);
}