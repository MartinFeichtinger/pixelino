#pragma once

namespace pixelino::driver {

enum class ButtonId {
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_A,
	KEY_B,
	KEY_SELECT,
	KEY_START,
	ONBOARD
};

enum class ButtonEvent {
	PRESS,
	CLICK,
	LONG_PRESS
};

// higher number = higher priority (executes first)
enum class HandlerPriority : int {
	LOW_PRIORITY		= 10,  // e.g. background analytics, logging
	NORMAL_PRIORITY		= 50,  // e.g. passive click sound feedback
	HIGH_PRIORITY		= 100, // e.g. serviceCLI, settings menu
	CRITICAL_PRIORITY	= 200  // e.g. system reset, emergency hardware overrides
};

inline const char* buttonIdToString (ButtonId id) {
	switch (id)
	{
		case ButtonId::KEY_UP:		return "KEY_UP";
		case ButtonId::KEY_DOWN:	return "KEY_DOWN";
		case ButtonId::KEY_LEFT:	return "KEY_LEFT";
		case ButtonId::KEY_RIGHT:	return "KEY_RIGHT";
		case ButtonId::KEY_A:		return "KEY_A";
		case ButtonId::KEY_B:		return "KEY_B";
		case ButtonId::KEY_SELECT:	return "KEY_SELECT";
		case ButtonId::KEY_START:	return "KEY_START";
		case ButtonId::ONBOARD:		return "ONBOARD";
		default:					return "UNKN";
	}
}

inline const char* buttonEventToString (ButtonEvent event) {
	switch (event) {
		case ButtonEvent::PRESS:		return "PRESS";
		case ButtonEvent::CLICK:		return "CLICK";
		case ButtonEvent::LONG_PRESS:	return "LONG_PRESS";
		default:						return "UNKN";
	}
}



} // namespace pixelino::driver