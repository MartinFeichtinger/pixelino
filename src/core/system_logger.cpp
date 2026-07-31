#include "core/system_logger.hpp"
#include "core/error_types.hpp"
#include "core/system_events.hpp"
#include <cstdint>
#include <Arduino.h>

namespace pixelino::core {

void SystemLogger::log(SystemLogEntry entry) {
	entry.timestamp = millis();

	std::uint8_t head = m_count % max_log_entries;
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

void SystemLogger::logDriverEvent(const char* tag, const char* message) {
    SystemLogEntry entry;
    entry.source = LogSource::DRIVER;
    entry.payload.driver.tag = tag;
    entry.payload.driver.message = message;
    log(entry);
}

void SystemLogger::printLogHistory() const {
    if (m_count == 0) {
        Serial.println("log is empty");
        return;
    }

	std::uint8_t head = m_count % max_log_entries;
	std::uint8_t logs = (m_count <= max_log_entries) ? m_count : max_log_entries; // amount of curretly stored log entries
    std::uint8_t tail = (m_count > max_log_entries) ? head : 0;

    for (std::uint8_t i = 0; i < logs; i++) {
        std::uint8_t index = (tail + i) % max_log_entries;
        const SystemLogEntry& entry = m_log[index];

		printFormattedTimestamp(entry.timestamp);
		Serial.print(" ");


        switch (entry.source) {
            case LogSource::ERROR:
				// change serial output color dependig on error (ascii escape sequences)
				if (getErrorLevel(entry.payload.error.code) >= ErrorLevel::ERROR) Serial.printf("\e[31m");			// red
				else if (getErrorLevel(entry.payload.error.code) == ErrorLevel::WARNING) Serial.printf("\e[33m");	// yellow
				else if (getErrorLevel(entry.payload.error.code) == ErrorLevel::INFO) Serial.printf("\e[1m]");		// bold

                Serial.printf("[%s] \t %s\n",
                    levelToString(getErrorLevel(entry.payload.error.code)),
                    getErrorMessage(entry.payload.error.code));

				Serial.printf("\e[0m");	// reset serial output format
                break;

			case LogSource::SYSTEM:
				if (entry.payload.system.event == SystemEvent::ERROR_MODE_CHANGED) {
					Serial.printf("[SYSTEM] \t %s TO %s\n",
						getSystemEventMessage(entry.payload.system.event),
						modeToString(entry.payload.system.errorMode));
				} else {
					Serial.printf("[SYSTEM] \t %s\n",
						getSystemEventMessage(entry.payload.system.event));
				}
				break;

			case LogSource::DRIVER:
                Serial.printf("[%s] \t %s\n",
                    entry.payload.driver.tag,
                    entry.payload.driver.message);
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

// helper function to format timestamp
inline void SystemLogger::printFormattedTimestamp(uint32_t ms) const {
    uint32_t seconds = ms / 1000;
    uint32_t minutes = seconds / 60;
    
    uint32_t rem_seconds = seconds % 60;
    uint32_t rem_ms = ms % 1000;

    // output format: [MM:SS.mmm]
    Serial.printf("[%02lu:%02lu.%03lu]", minutes, rem_seconds, rem_ms);
}

} // namespace pixelino::core