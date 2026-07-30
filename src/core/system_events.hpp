#pragma once
#include <cstdint>

namespace pixelino::core {

enum class SystemEvent : std::uint8_t {
	NONE = 0,
	SETUP,
	LOG_CLEARD,
	ERROR_MODE_CHANGED
};

inline const char* getSystemEventMessage(SystemEvent event) {
	switch (event) {
		case SystemEvent::SETUP:				return "SETUP FINISHED";
		case SystemEvent::LOG_CLEARD:			return "LOG CLEARD";
		case SystemEvent::ERROR_MODE_CHANGED:	return "ERROR_HANDLER CHANGED MODE";
		default:								return "UNKNOWN EVENT";
	}
}

} // namespace pixelino::core