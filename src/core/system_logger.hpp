#pragma once

#include "core/error_types.hpp"
#include "core/system_events.hpp"
#include "driver/button_types.hpp"
#include <cstdint>
#include <Arduino.h>

namespace pixelino::core {

enum class LogSource : std::uint8_t {
    BUTTON,
    ERROR,
    SYSTEM
};

struct SystemLogEntry {
    uint32_t timestamp;
    LogSource source;
    
    // Union size is determined by the largest struct inside it.
    // Button: 2 bytes. Error: 1 byte.
    // Total struct size: 4 (time) + 1 (source) + 2 (union) + 1 (padding) = 8 bytes per entry.
    union {
        struct {
            driver::ButtonId id;
            driver::ButtonEvent event;
        } button;
        
        struct {
            ErrorCode code;
        } error;

		struct {
			SystemEvent event;
            union {
                ErrorMode errorMode;
            };
		} system;
    } payload;
};

class SystemLogger {
public:
    static SystemLogger& getInstance() {
        static SystemLogger instance;
        return instance;
    }

    SystemLogger(const SystemLogger&) = delete;
    void operator=(const SystemLogger&) = delete;

    void setServiceMode(bool enabled) { m_serviceMode = enabled; }
    void setParsingMode(bool enabled) { m_parsingMode = enabled; }

	// log manipulation functions
    void printLogHistory() const;
    void clearLog();

    // logging from diffenent souces
	void logError(ErrorCode code, ErrorMode mode);
	void logSystemEvent(SystemEvent event);
    void logSystemEvent(SystemEvent event, ErrorMode mode);
    void logButtonEvent(driver::ButtonId id, driver::ButtonEvent event);

private:
    SystemLogger() = default;

	void log(SystemLogEntry entry);

    static constexpr size_t max_log_entries = 32;
    SystemLogEntry m_log[max_log_entries];
    size_t m_count = 0;		// total counted log entries

    inline void printFormattedTimestamp(uint32_t ms) const;

    bool m_serviceMode = false;
    bool m_parsingMode = false;
};

} // namespace pixelino::core