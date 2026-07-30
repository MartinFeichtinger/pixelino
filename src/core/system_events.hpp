#pragma once
#include <cstdint>

namespace pixelino::core {

enum class SystemEvent : std::uint8_t {
	NONE = 0,
	SETUP,
	LOG_CLEARD,
	ERROR_MODE_CHANGED,
	SERVICE_CLI_ACTIVATED,
	SERVICE_CLI_DEACTIVATED
};

inline const char* getSystemEventMessage(SystemEvent event) {
	switch (event) {
		case SystemEvent::SETUP:					return "SETUP FINISHED";
		case SystemEvent::LOG_CLEARD:				return "LOG CLEARD";
		case SystemEvent::ERROR_MODE_CHANGED:		return "ERROR_HANDLER CHANGED MODE";
		case SystemEvent::SERVICE_CLI_ACTIVATED:	return "SERVICE_CLI ACTIVATED";
		case SystemEvent::SERVICE_CLI_DEACTIVATED:	return "SERVICE_CLI DEACTIVATED";
		default:									return "UNKNOWN EVENT";
	}
}

} // namespace pixelino::core