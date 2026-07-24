#include "serviceCLI.hpp"

ServiceCLI& ServiceCLI::getInstance() {
    static ServiceCLI instance; // Guaranteed single instance
    return instance;
}

ServiceCLI::ServiceCLI() {
    m_cli.addCommand("ping", pingCallback);
    m_cli.addCommand("exit", exitCallback);
    m_cli.setOnError(errorCallback);
}

void ServiceCLI::begin() {
	pinMode(config::pin::onboard_led, OUTPUT);
}

void ServiceCLI::activate(void) {
    if (m_isActive) return;

	// flush the serial input buffer
	while (Serial.available()) {
		Serial.read();
	}

    m_isActive = true;
    digitalWrite(config::pin::onboard_led, HIGH);
    Serial.println("\n==================================== serviceCLI activated ====================================");
    Serial.print("serviceCLI-esp32> ");
}

void ServiceCLI::deactivate(void) {
    if (!m_isActive) return;

    m_isActive = false;
    digitalWrite(config::pin::onboard_led, LOW);
    Serial.println("=================================== serviceCLI deactivated ===================================");
	Serial.println();
}

void ServiceCLI::toggle(void) {
    if (m_isActive) {
		Serial.println();
        deactivate();
    } else {
        activate();
    }
}

void ServiceCLI::tick(void) {
    if (!m_isActive) return;

    static String inputBuffer = "";

    while (Serial.available()) {
        char c = Serial.read();

        if (c == '\n') {
            Serial.println();
            
            if (inputBuffer.length() > 0) {
                m_cli.parse(inputBuffer);
                inputBuffer = ""; // Reset buffer
            }

            if (m_isActive) {
                Serial.print("serviceCLI-esp32> ");
            }
        }
        // handle backspace (0x08 / '\b' or 0x7F / DEL)
        else if (c == '\b' || c == 0x7F) {
            if (inputBuffer.length() > 0) {
                inputBuffer.remove(inputBuffer.length() - 1); // delete from internal memory
                
                // ERASE ON SCREEN: Backspace -> Space -> Backspace
                Serial.print("\b \b"); 
            }
        }
        // handle normal characters
        else if (c >= 32 && c <= 126) { // printable ASCII range
            inputBuffer += c;
            Serial.print(c); // echo character back to screen immediately
        }
    }
}

void ServiceCLI::exitCallback(cmd* c) {
    // Calling non-static deactivate via the singleton instance!
    ServiceCLI::getInstance().deactivate();
}

void ServiceCLI::pingCallback(cmd* c) {
    Serial.println("pong");
}

void ServiceCLI::errorCallback(cmd_error* e) {
    CommandError cmdError(e);
    Serial.print("ERROR: ");
    Serial.println(cmdError.toString());

    if (cmdError.hasCommand()) {
        Serial.print("Did you mean \"");
        Serial.print(cmdError.getCommand().toString());
        Serial.println("\"?");
    }
}