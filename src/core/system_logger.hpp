#pragma once

#include "core/error_types.hpp"
#include "core/system_events.hpp"
#include <cstdint>

namespace pixelino::core {

enum class LogSource : std::uint8_t {
    ERROR,
    SYSTEM,
    DRIVER
};

struct SystemLogEntry {
    uint32_t timestamp;
    LogSource source;
    
    // Union size is determined by the largest struct inside it.
    // error: 1 byte, system: 2 bytes, driver: 8 bytes
    // Total struct size: 4 (time) + 1 (source) + 8 (union) + 3 (padding) = 16 bytes per entry.
    union {        
        struct {
            ErrorCode code;
        } error;

		struct {
			SystemEvent event;
            union {
                ErrorMode errorMode;
            };
		} system;

        struct {
            const char* tag; 
            const char* message;
        } driver;

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
    void logDriverEvent(const char* tag, const char* message);

private:
    SystemLogger() = default;

	void log(SystemLogEntry entry);

    static constexpr std::uint8_t max_log_entries = 32;
    SystemLogEntry m_log[max_log_entries];
    std::uint16_t m_count = 0;		// total counted log entries

    inline void printFormattedTimestamp(uint32_t ms) const;

    bool m_serviceMode = false;
    bool m_parsingMode = false;
};

} // namespace pixelino::core