#include "serviceCLI.hpp"

#include "core/config.hpp"
#include "core/types.hpp"
#include "core/error_handler.hpp"
#include "core/error_types.hpp"
#include "core/system_logger.hpp"

#include "driver/display.hpp"
#include "driver/button_manager.hpp"

#include <SimpleCLI.h>
#include <Arduino.h>

namespace pixelino::app {

void ServiceCLI::begin() {
	Serial.begin(115200);
	pinMode(core::config::gpio::onboard_led, OUTPUT);

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

	Command logCmd = m_cli.addCommand("log", logCallback);
	logCmd.addPositionalArgument("action", "show"); // "show", "clear"
	logCmd.setDescription(" Allows to access and clear the error log (show, clear).");

	Command errorCmd = m_cli.addCommand("errorHandler", errorHandlerCallback);
	errorCmd.addPositionalArgument("action", "getMode"); // sub-action: "getMode" or "setMode"
	errorCmd.addPositionalArgument("mode", "");          // argument for setMode
	errorCmd.setDescription("Manage error handling (getMode | setMode <silent, log_only, log_life, brodcast, crash_on_fatal, crash_on_error, crash_on_warning>).");

	m_cli.setOnError(cliErrorCallback);

	core::SystemLogger::getInstance().logEvent(core::LogSource::APP, "CLI", "CLI INITIATED");
}

void ServiceCLI::activate(void) {
	if (m_isActive) return;

	// flush the serial input buffer
	while (Serial.available()) {
		Serial.read();
	}

	m_isActive = true;
	digitalWrite(core::config::gpio::onboard_led, HIGH);
	core::SystemLogger::getInstance().logEvent(core::LogSource::APP, "CLI", "CLI ACTIVATED");
	Serial.println("\n====================================== serviceCLI activated =======================================");
	Serial.print("serviceCLI-esp32> ");
}

void ServiceCLI::deactivate(void) {
	if (!m_isActive) return;

	m_isActive = false;
	digitalWrite(core::config::gpio::onboard_led, LOW);
	core::SystemLogger::getInstance().logEvent(core::LogSource::APP, "CLI", "CLI DEACTIVATED");
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
				m_parsingInput = true;
				m_cli.parse(m_inputBuffer);
				m_parsingInput = false;
				m_inputBuffer = ""; // Reset buffer
			}
			if (m_isActive) Serial.print("serviceCLI-esp32> ");
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

void ServiceCLI::printLiveError(core::ErrorCode code, core::ErrorMode mode) {
    if ((m_isActive && mode == core::ErrorMode::LOG_LIVE) || mode >= core::ErrorMode::BRODCAST) {
        
        // ANSI escape sequence magic: 
        // \r moves cursor to the beginning of the line
        // \e[K clears everything from the cursor to the end of the line
        Serial.print("\r\e[K"); 

        Serial.print("[LIVE_");
        Serial.print(core::levelToString(core::getErrorLevel(code)));
        Serial.print("]\t");
        Serial.print(core::getErrorMessage(code)); 
		Serial.print("\n");
        
        if (m_isActive && !m_parsingInput) {
			// reprint promt
			Serial.print("serviceCLI-esp32> ");
			Serial.print(m_inputBuffer);
		}
    }
}

void ServiceCLI::printSystemCrashMsg() {
	Serial.println("\n*** FATAL ERROR: SYSTEM HALTED ***");

    Serial.println("*** LOG HISTORY ***");
    printLogHistory();

    Serial.println("\n*** FATAL ERROR: SYSTEM HALTED ***");
}

// Add the migrated log history printer
void ServiceCLI::printLogHistory() {
    auto& logger = core::SystemLogger::getInstance();
    std::uint8_t logs = logger.getStoredLogCount();
    std::uint16_t totalCount = logger.getTotalLogCount();

    if (totalCount == 0) {
        Serial.println("log is empty");
        return;
    }

    for (std::uint8_t i = 0; i < logs; i++) {
        const core::SystemLogEntry& entry = logger.getLogEntry(i);

        if (entry.source == core::LogSource::ERROR_HANDLER) {
            core::ErrorLevel lvl = core::getErrorLevel(entry.payload.error.code);
            if (lvl >= core::ErrorLevel::ERROR) Serial.printf("\e[31m");          // red
            else if (lvl == core::ErrorLevel::WARNING) Serial.printf("\e[33m");   // yellow
            else if (lvl == core::ErrorLevel::INFO) Serial.printf("\e[1m");       // bold

            printFormattedTimestamp(entry.timestamp);
            Serial.printf(" [%s] \t %s\n",
                core::levelToString(lvl),
                core::getErrorMessage(entry.payload.error.code));

            Serial.printf("\e[0m"); // reset serial output format
        } else {
            printFormattedTimestamp(entry.timestamp);
            Serial.printf(" [%s] \t %s\n",
                entry.payload.event.tag,
                entry.payload.event.message);
        }
    }

    if (totalCount > 32) { // max_log_entries
        Serial.print("log overflowed: ");
        Serial.print(totalCount);
        Serial.print(" LOGS, ");
		Serial.print(core::SystemLogger::max_log_entries);
		Serial.println(" MAX_LOG_ENTRIES");
    }
}

void ServiceCLI::printFormattedTimestamp(uint32_t ms) {
    uint32_t seconds = ms / 1000;
    uint32_t minutes = seconds / 60;
    uint32_t rem_seconds = seconds % 60;
    uint32_t rem_ms = ms % 1000;
    Serial.printf("[%02lu:%02lu.%03lu]", minutes, rem_seconds, rem_ms);
}

// CLI callbacks
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

	driver::Display& display = driver::Display::getInstance();

	if (action.equalsIgnoreCase("fill")) {
		display.fill(core::Color(r, g, b));
		display.show();
	} 
	else if (action.equalsIgnoreCase("setPixel")) {
		display.setPixel(x, y, core::Color(r, g, b));
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

void ServiceCLI::logCallback(cmd* c)
{
	Command cmd(c);

	String action = cmd.getArgument("action").getValue();

	if (action.equalsIgnoreCase("show")) {
		ServiceCLI::getInstance().printLogHistory();
	}
	else if(action.equalsIgnoreCase("clear")) {
		core::SystemLogger::getInstance().clearLog();
		Serial.println("system log cleared");
	}
}

void ServiceCLI::errorHandlerCallback(cmd* c) {
	Command cmd(c);
	String action = cmd.getArgument("action").getValue();

	if (action.equalsIgnoreCase("getMode")) {
		core::ErrorMode currentMode = core::ErrorHandler::getInstance().getMode();
		Serial.println(core::modeToString(currentMode));
	} 
	else if (action.equalsIgnoreCase("setMode")) {
		String mode = cmd.getArgument("mode").getValue();

		if (mode.equalsIgnoreCase("silent"))				core::ErrorHandler::getInstance().setMode(core::ErrorMode::SILENT);
		else if (mode.equalsIgnoreCase("log_only"))			core::ErrorHandler::getInstance().setMode(core::ErrorMode::LOG_ONLY);
		else if (mode.equalsIgnoreCase("log_live"))			core::ErrorHandler::getInstance().setMode(core::ErrorMode::LOG_LIVE);
		else if (mode.equalsIgnoreCase("brodcast"))			core::ErrorHandler::getInstance().setMode(core::ErrorMode::BRODCAST);
		else if (mode.equalsIgnoreCase("crash_on_fatal"))	core::ErrorHandler::getInstance().setMode(core::ErrorMode::CRASH_ON_FATAL);
		else if (mode.equalsIgnoreCase("crash_on_error"))	core::ErrorHandler::getInstance().setMode(core::ErrorMode::CRASH_ON_ERROR);
		else {
			Serial.println("[CLI_ERROR]: Missing or invalid mode. Options: <silent, log_only, log_live, brodcast, crash_on_fatal, crash_on_error, crash_on_warning>");
		}
	} 
	else {
		Serial.println("[CLI_ERROR]: Unknown subcommand. Usage: 'error getMode' or 'error setMode <mode>'");
	}
}

void ServiceCLI::cliErrorCallback(cmd_error* e) {
	CommandError cmdError(e);
	Serial.print("[CLI_ERROR]: ");
	Serial.println(cmdError.toString());

	if (cmdError.hasCommand()) {
		Serial.print("Did you mean \"");
		Serial.print(cmdError.getCommand().toString());
		Serial.println("\"?");
	}
}

}