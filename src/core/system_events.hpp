#pragma once
#include <cstdint>

namespace pixelino::core {

enum class SystemEvent : std::uint8_t {
	NONE = 0,
	SETUP,
	LOG_CLEARD
};

inline const char* getSystemEventMessage(SystemEvent event) {
	switch (event) {
		case SystemEvent::SETUP:		return "system setup finished";
		case SystemEvent::LOG_CLEARD:	return "system log cleared";
		default:						return "unknown event";
	}
}

} // namespace pixelino::core