#include "core/system_logger.hpp"
#include "core/error_types.hpp"
#include <cstdint>
#include <Arduino.h>

namespace pixelino::core {

void SystemLogger::logError(ErrorCode code) {	
	SystemLogEntry entry;
	entry.source = LogSource::ERROR_HANDLER;
	entry.payload.error.code = code;
	log(entry);
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

void SystemLogger::clearLog() {
	m_count = 0;
	logEvent(LogSource::SYSTEM, "SYSTEM_LOG CLEARED");
}

std::uint8_t SystemLogger::getStoredLogCount() const {
    return (m_count <= max_log_entries) ? m_count : max_log_entries;
}

std::uint16_t SystemLogger::getTotalLogCount() const {
    return m_count;
}

const SystemLogEntry& SystemLogger::getLogEntry(std::uint8_t i) const {
    std::uint8_t head = m_count % max_log_entries;
    std::uint8_t tail = (m_count > max_log_entries) ? head : 0;
    std::uint8_t index = (tail + i) % max_log_entries;
    return m_log[index];
}

const char* SystemLogger::logSourceToString(LogSource source) {
    switch (source) {
        case LogSource::ERROR_HANDLER:  return "ERR_HNDL";
        case LogSource::SYSTEM:         return "SYSTEM";
        case LogSource::DRIVER:         return "DRIVER";
        case LogSource::APP:            return "APP";
        default:                        return "UNKN";
    }
}

} // namespace pixelino::core