#include "serviceCLI.hpp"

ServiceCLI::ServiceCLI() {
	// initiate all commands and connect them to their callbacks
	m_cli.addCommand("ping", pingCallback);
	m_cli.setOnError(errorCallback);
}

void ServiceCLI::activate(void) {
	Serial.println("==================================== serviceCLI activated ====================================");
	Serial.print("serviceCLI-esp32> ");
}

void ServiceCLI::deactivate(void) {
	Serial.println("");
	Serial.println("=================================== serviceCLI deactivated ===================================");
}

void ServiceCLI::tick(void) {
	if (Serial.available()) {
		String input = Serial.readStringUntil('\n');
		m_cli.parse(input);
		Serial.print("serviceCLI-esp32> ");
	}
}

void ServiceCLI::errorCallback(cmd_error* e) {
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

void ServiceCLI::pingCallback(cmd* c) {
	Serial.println("pong");
}