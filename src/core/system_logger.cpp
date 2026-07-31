#include "core/system_logger.hpp"
#include "core/error_types.hpp"
#include <cstdint>
#include <Arduino.h>

namespace pixelino::core {

void SystemLogger::logError(ErrorCode code,  ErrorMode mode) {	
	// error logging
	SystemLogEntry entry;
	entry.source = LogSource::ERROR_HANDLER;
	entry.payload.error.code = code;
	log(entry);

	// serial print if live output enabled
	if ((m_serviceMode && mode == ErrorMode::LOG_LIVE) || mode >= ErrorMode::BRODCAST) {
		if (!m_parsingMode) Serial.print("\n");
		Serial.print("[LIVE_");
        Serial.print(levelToString(getErrorLevel(code)));
        Serial.print("]\t");
        Serial.print(getErrorMessage(code));
		if (!m_parsingMode && m_serviceMode) Serial.print("\nserviceCLI-esp32> ");
	}
}

void SystemLogger::logEvent(LogSource source, const char* message) {
	logEvent(source, logSourceToString(source), message);
}

void SystemLogger::logEvent(LogSource source, const char* tag, const char* message) {
    SystemLogEntry entry;
    entry.source = source;
    entry.payload.event.tag = tag;
    entry.payload.event.message = message;
    log(entry);
}

void SystemLogger::log(SystemLogEntry entry) {
	entry.timestamp = millis();

	std::uint8_t head = m_count % max_log_entries;
	m_log[head] = entry;
	m_count++;
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

		if (entry.source == LogSource::ERROR_HANDLER) {
			// change serial output color dependig on error (using ascii escape sequences)
			if (getErrorLevel(entry.payload.error.code) >= ErrorLevel::ERROR) Serial.printf("\e[31m");			// red
			else if (getErrorLevel(entry.payload.error.code) == ErrorLevel::WARNING) Serial.printf("\e[33m");	// yellow
			else if (getErrorLevel(entry.payload.error.code) == ErrorLevel::INFO) Serial.printf("\e[1m]");		// bold

			printFormattedTimestamp(entry.timestamp);
			Serial.printf(" [%s] \t %s\n",
				levelToString(getErrorLevel(entry.payload.error.code)),
				getErrorMessage(entry.payload.error.code));

			Serial.printf("\e[0m");	// reset serial output format
		}
		else {
			printFormattedTimestamp(entry.timestamp);
			Serial.printf(" [%s] \t %s\n",
				entry.payload.event.tag,
				entry.payload.event.message);
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
	logEvent(LogSource::SYSTEM, "SYSTEM_LOG CLEARED");
	Serial.println("system log cleared");
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

const char* SystemLogger::logSourceToString(LogSource source) {
	switch (source) {
		case LogSource::ERROR_HANDLER:	return "ERR_HNDL";
		case LogSource::SYSTEM: 		return "SYSTEM";
		case LogSource::DRIVER: 		return "DRIVER";
		case LogSource::APP:			return "APP";
		default:						return "UNKN";
	}
}


} // namespace pixelino::core