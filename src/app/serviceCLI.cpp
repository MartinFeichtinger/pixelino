#include "serviceCLI.h"
#include <Arduino.h>
#include <SimpleCLI.h>	// props to spacehuhn for this amazing library

SimpleCLI cli;


void errorCallback(cmd_error* e);
void pingCallback(cmd* c);


void SERVICE_CLI_init(void)
{
	// initiate all commands and connect them to their callbacks
	cli.addCommand("ping", pingCallback);
	cli.setOnError(errorCallback);
}


void SERVICE_CLI_activate(void)
{
	Serial.println("==================================== serviceCLI activated ====================================");
	Serial.print("serviceCLI-esp32> ");
}


void SERVICE_CLI_deactivate(void)
{
	Serial.println("=================================== serviceCLI deactivated ===================================");
}


void SERVICE_CLI_runService(void)
{
	if(Serial.available())
	{
		String input = Serial.readStringUntil('\n');
		cli.parse(input);
		Serial.print("serviceCLI-esp32> ");
	}
}


void errorCallback(cmd_error* e)
{
    CommandError cmdError(e); // Create wrapper object

    // Print error
    Serial.print("ERROR: ");
    Serial.println(cmdError.toString());

    // Print command usage
    if (cmdError.hasCommand()) {
        Serial.print("Did you mean \"");
        Serial.print(cmdError.getCommand().toString());
        Serial.println("\"?");
    }
}

void pingCallback(cmd* c)
{
	Serial.println("pong");
}