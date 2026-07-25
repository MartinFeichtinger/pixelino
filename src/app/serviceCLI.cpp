#include "serviceCLI.hpp"
#include "types.hpp"
#include "driver/display.hpp"

ServiceCLI& ServiceCLI::getInstance() {
    static ServiceCLI instance; // Guaranteed single instance
    return instance;
}

void ServiceCLI::begin() {
	pinMode(config::pin::onboard_led, OUTPUT);

	Command pingCmd = m_cli.addCommand("ping", pingCallback);
	pingCmd.setDescription(" Response with pong to test CLI connection.");

    Command exitCmd = m_cli.addCommand("exit", exitCallback);
	exitCmd.setDescription(" Deactivates the service CLI mode.");

	Command helpCmd = m_cli.addCommand("h/elp", helpCallback);
	helpCmd.setDescription(" Display all available commands and there usage.");

    Command displayCmd = m_cli.addCommand("display", displayCallback);
    displayCmd.addPositionalArgument("action"); // "fill", "setPixel", "clear"
    displayCmd.addArgument("x", "0");
    displayCmd.addArgument("y", "0");
    displayCmd.addArgument("r", "0");
    displayCmd.addArgument("g", "0");
    displayCmd.addArgument("b", "0");
	displayCmd.setDescription(" Allows to manipulate the display via the serviceCLI (fill, setPixel, clear).");

	m_cli.setOnError(errorCallback);
}

void ServiceCLI::activate(void) {
    if (m_isActive) return;

	// flush the serial input buffer
	while (Serial.available()) {
		Serial.read();
	}

    m_isActive = true;
    digitalWrite(config::pin::onboard_led, HIGH);
    Serial.println("\n====================================== serviceCLI activated =======================================");
    Serial.print("serviceCLI-esp32> ");
}

void ServiceCLI::deactivate(void) {
    if (!m_isActive) return;

    m_isActive = false;
    digitalWrite(config::pin::onboard_led, LOW);
    Serial.println("===================================== serviceCLI deactivated ======================================");
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

    while (Serial.available()) {
        char c = Serial.read();

        if (c == '\n') {
            Serial.println();
            
            if (m_inputBuffer.length() > 0) {
                m_cli.parse(m_inputBuffer);
                m_inputBuffer = ""; // Reset buffer
            }

            if (m_isActive) {
                Serial.print("serviceCLI-esp32> ");
            }
        }
        // handle backspace (0x08 / '\b' or 0x7F / DEL)
        else if (c == '\b' || c == 0x7F) {
            if (m_inputBuffer.length() > 0) {
                m_inputBuffer.remove(m_inputBuffer.length() - 1); // delete from internal memory
                
                // ERASE ON SCREEN: Backspace -> Space -> Backspace
                Serial.print("\b \b"); 
            }
        }
        // handle normal characters
        else if (c >= 32 && c <= 126) { // printable ASCII range
            m_inputBuffer += c;
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

void ServiceCLI::helpCallback(cmd* c) {
	Serial.println();
    Serial.print(getInstance().m_cli.toString());    
}

void ServiceCLI::displayCallback(cmd* c) {
    Command cmd(c);
    
    String action = cmd.getArgument("action").getValue();
    
    uint8_t r = cmd.getArgument("r").getValue().toInt();
    uint8_t g = cmd.getArgument("g").getValue().toInt();
    uint8_t b = cmd.getArgument("b").getValue().toInt();
    uint8_t x = cmd.getArgument("x").getValue().toInt();
    uint8_t y = cmd.getArgument("y").getValue().toInt();

    engine::Display& display = engine::Display::getInstance();

    if (action.equalsIgnoreCase("fill")) {
        display.fill(engine::Color(r, g, b));
        display.show();
    } 
    else if (action.equalsIgnoreCase("setPixel")) {
        display.setPixel(x, y, engine::Color(r, g, b));
        display.show();
    } 
    else if (action.equalsIgnoreCase("clear")) {
        display.clear();
        display.show();
    }
	else {
		Serial.println("ERROR: Missing argument <fill, setPixel, clear>");
	}
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