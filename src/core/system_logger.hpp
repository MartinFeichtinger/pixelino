#pragma once

#include "core/error_types.hpp"
#include <cstdint>

namespace pixelino::core {

enum class LogSource : std::uint8_t {
    ERROR_HANDLER,
    SYSTEM,
    DRIVER,
    APP,
};

struct SystemLogEntry {
    uint32_t timestamp;
    LogSource source;

    union {
        struct {
            ErrorCode code;
        } error;

        struct {
            const char* tag; 
            const char* message;
        } event;

    } payload;
};

class SystemLogger {
public:
    // singelton definition
    static SystemLogger& getInstance() { static SystemLogger instance; return instance; }
    SystemLogger(const SystemLogger&) = delete;
    void operator=(const SystemLogger&) = delete;

    // special logger used by the error handler (for error logging use the error handler!)
	void logError(ErrorCode code, ErrorMode mode);  

    // universal logging function
    void logEvent(LogSource source, const char* message);
    void logEvent(LogSource source, const char* tag, const char* message);
    void printLogHistory() const;
    void clearLog();

    // relevant class states set via the serviceCLI modul for corect formatign
    void setServiceMode(bool enabled) { m_serviceMode = enabled; }
    void setParsingMode(bool enabled) { m_parsingMode = enabled; }

private:
    SystemLogger() = default;

	void log(SystemLogEntry entry);

    static constexpr std::uint8_t max_log_entries = 32;
    SystemLogEntry m_log[max_log_entries];
    std::uint16_t m_count = 0;		// total counted log entries

    inline void printFormattedTimestamp(uint32_t ms) const;
    static const char* logSourceToString(LogSource source);

    bool m_serviceMode = false;
    bool m_parsingMode = false;
};

} // namespace pixelino::core