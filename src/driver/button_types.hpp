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
	DOUBLE_CLICK,
	PRESS_START
};

// higher number = higher priority (executes first)
enum class HandlerPriority : int {
	LOW_PRIORITY		= 10,  // e.g. background analytics, logging
	NORMAL_PRIORITY		= 50,  // e.g. passive click sound feedback
	HIGH_PRIORITY		= 100, // e.g. serviceCLI, settings menu
	CRITICAL_PRIORITY	= 200  // e.g. system reset, emergency hardware overrides
};

} // namespace pixelino::driver