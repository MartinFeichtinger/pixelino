#pragma once
#include <cstdint>

namespace pixelino::core {

enum class ErrorLevel : std::uint8_t {
    INFO,
    WARNING,
    ERROR,
    FATAL
};

enum class ErrorMode : std::uint8_t {
    SILENT = 0,			// no reaprintction
    LOG_ONLY = 1,		// only log errors
	LOG_LIVE = 2,		// log errors an print them in service mode
	BRODCAST = 3,		// log and brodcast even when service mode is deactivated		
    CRASH_ON_FATAL = 4,	// brodcast error, set visual display info and halt the system at fatal error
	CRASH_ON_ERROR = 5	// -||- at normal error
};

enum class ErrorCode : std::uint8_t {
    NONE = 0,
    PIXEL_OUT_OF_BOUNDS,
};

// translates the 1-byte error code into a string on demand
inline const char* getErrorMessage(ErrorCode code) {
    switch (code) {
        case ErrorCode::PIXEL_OUT_OF_BOUNDS:	return "Display: Pixel out of bounds";
        default:								return "Unknown Error";
    }
}

inline ErrorLevel getErrorLevel(ErrorCode code) {
    switch (code) {
        case ErrorCode::PIXEL_OUT_OF_BOUNDS:	return ErrorLevel::ERROR;
        default:								return ErrorLevel::INFO;
    }
}

// translate error level to string for serial output
inline const char* levelToString(ErrorLevel level) {
    switch (level) {
        case ErrorLevel::INFO:			return "INFO";
        case ErrorLevel::WARNING:		return "WARN";
        case ErrorLevel::ERROR:			return "ERROR";
        case ErrorLevel::FATAL:			return "FATAL";
        default:						return "UNKN";
    }
}

// translate error mode to string for serial output
inline const char* modeToString (ErrorMode mode) {
	switch (mode) {
		case ErrorMode::SILENT:			return "SILENT";
		case ErrorMode::LOG_ONLY:		return "LOG_ONLY";
		case ErrorMode::LOG_LIVE:		return "LOG_LIVE";
		case ErrorMode::BRODCAST:		return "BRODCAST";
		case ErrorMode::CRASH_ON_FATAL:	return "CRASH_ON_FATAL";
		case ErrorMode::CRASH_ON_ERROR:	return "CRASH_ON_ERROR";
		default:						return "UNKN";
	}
}

} // namespace pixelino::core