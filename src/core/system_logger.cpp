#include "core/system_logger.hpp"
#include "core/error_types.hpp"
#include "core/system_events.hpp"
#include "driver/button_types.hpp"
#include <cstdint>

namespace pixelino::core {

void SystemLogger::log(SystemLogEntry entry) {
	entry.timestamp = millis();

	size_t head = m_count % max_log_entries;
	m_log[head] = entry;
	m_count++;
}

void SystemLogger::logError(ErrorCode code,  ErrorMode mode) {	
	// error logging
	SystemLogEntry entry;
	entry.source = LogSource::ERROR;
	entry.payload.error.code = code;
	log(entry);

	// serial print if live output enabled
	if ((m_serviceMode && mode == ErrorMode::LOG_LIVE) || mode >= ErrorMode::BRODCAST) {
		if (!m_parsingMode) Serial.print("\n");
		Serial.print("[LIVE_");
        Serial.print(levelToString(getErrorLevel(code)));
        Serial.print("]\t");
        Serial.println(getErrorMessage(code));
		if (!m_parsingMode) Serial.print("serviceCLI-esp32> ");
	}
}

void SystemLogger::logSystemEvent(SystemEvent event) {
	SystemLogEntry entry;
	entry.source = LogSource::SYSTEM;
	entry.payload.system.event = event;
	log(entry);
}

void SystemLogger::logSystemEvent(SystemEvent event, ErrorMode mode) {
    SystemLogEntry entry;
    entry.source = LogSource::SYSTEM;
    entry.payload.system.event = event;
    entry.payload.system.errorMode = mode;
    log(entry);
}

void SystemLogger::logButtonEvent(driver::ButtonId id, driver::ButtonEvent event) {
	SystemLogEntry entry;
	entry.source = LogSource::BUTTON;
	entry.payload.button.id = id;
	entry.payload.button.event = event;
	log(entry);
}

void SystemLogger::printLogHistory() const {
    if (m_count == 0) {
        Serial.println("log is empty");
        return;
    }

	size_t head = m_count % max_log_entries;
	size_t logs = (m_count <= max_log_entries) ? m_count : max_log_entries; // amount of curretly stored log entries
    size_t tail = (m_count > max_log_entries) ? head : 0;

    for (size_t i = 0; i < logs; i++) {
        size_t index = (tail + i) % max_log_entries;
        const SystemLogEntry& entry = m_log[index];

        switch (entry.source) {
            case LogSource::ERROR:
				// change serial output color dependig on error (ascii escape sequences)
				if (getErrorLevel(entry.payload.error.code) >= ErrorLevel::ERROR) Serial.printf("\e[31m");			// red
				else if (getErrorLevel(entry.payload.error.code) == ErrorLevel::WARNING) Serial.printf("\e[33m");	// yellow
				else if (getErrorLevel(entry.payload.error.code) == ErrorLevel::INFO) Serial.printf("\e[1m]");		// bold

                Serial.printf("[%08lu] [%s] \t %s\n",
					entry.timestamp,
                    levelToString(getErrorLevel(entry.payload.error.code)),
                    getErrorMessage(entry.payload.error.code));

				Serial.printf("\e[0m");	// reset serial output format
                break;

			case LogSource::SYSTEM:
				if (entry.payload.system.event == SystemEvent::ERROR_MODE_CHANGED) {
					Serial.printf("[%08lu] [SYSTEM] \t %s TO %s\n",
						entry.timestamp,
						getSystemEventMessage(entry.payload.system.event),
						modeToString(entry.payload.system.errorMode));
				} else {
					Serial.printf("[%08lu] [SYSTEM] \t %s\n",
						entry.timestamp,
						getSystemEventMessage(entry.payload.system.event));
				}
				break;

			case LogSource::BUTTON:
                Serial.printf("[%08lu] [BUTTON] \t %s -> %s\n",
                    entry.timestamp,
                    buttonIdToString(entry.payload.button.id),
                    buttonEventToString(entry.payload.button.event));
                break;
        }
    }
	if (m_count > max_log_entries)
	{
		Serial.print("log overflowed: ");
		Serial.print(m_count);
		Serial.print(" LOGS, ");
		Serial.print(max_log_entries);
		Serial.println(" MAX_LOG_ENTRIES");
	}
}

void SystemLogger::clearLog() {
	m_count = 0;
	logSystemEvent(SystemEvent::LOG_CLEARD);
	Serial.println("error log cleared");
}

} // namespace pixelino::core