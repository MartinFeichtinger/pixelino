#include "driver/button_manager.hpp"
#include "core/config.hpp"
#include <OneButton.h>

namespace pixelino::driver {

ButtonManager::ButtonManager() : m_buttons{{
	{ ButtonId::KEY_UP,			OneButton(core::config::gpio::key_up, true, true) },
	{ ButtonId::KEY_DOWN,		OneButton(core::config::gpio::key_down, true, true) },
	{ ButtonId::KEY_LEFT,		OneButton(core::config::gpio::key_left, true, true) },
	{ ButtonId::KEY_RIGHT,		OneButton(core::config::gpio::key_right, true, true) },
	{ ButtonId::KEY_A,			OneButton(core::config::gpio::key_a, true, true) },
	{ ButtonId::KEY_B,			OneButton(core::config::gpio::key_b, true, true) },
	{ ButtonId::KEY_SELECT,		OneButton(core::config::gpio::key_select, true, true) },
	{ ButtonId::KEY_START,		OneButton(core::config::gpio::key_start, true, true) },
	{ ButtonId::ONBOARD,		OneButton(core::config::gpio::onboard_button, true, true) }
}} {}

void ButtonManager::begin() {
	for (ButtonBinding& binding : m_buttons) {
		// give the binding a reference back to this ButtonManager instance
		binding.mgr = this;

		// press event (statless lamda!)
		binding.button.attachPress([](void* ctx) {
			ButtonBinding* b = static_cast<ButtonBinding*>(ctx);
			if (b && b->mgr) {
				b->mgr->dispatchEvent(b->id, ButtonEvent::PRESS);
			}
		}, &binding); // <-- pass the address of the binding as the context

		// click event (stateless lambda!)
		binding.button.attachClick([](void* ctx) {
			// cast the raw void* back into our ButtonBinding struct
			ButtonBinding* b = static_cast<ButtonBinding*>(ctx);
			if (b && b->mgr) {
				b->mgr->dispatchEvent(b->id, ButtonEvent::CLICK);
			}
		}, &binding);

		// double click event (stateless lambda!)
		binding.button.attachDoubleClick([](void* ctx) {
			// cast the raw void* back into our ButtonBinding struct
			ButtonBinding* b = static_cast<ButtonBinding*>(ctx);
			if (b && b->mgr) {
				b->mgr->dispatchEvent(b->id, ButtonEvent::DOUBLE_CLICK);
			}
		}, &binding);

		// long press event (stateless lambda!)
		binding.button.attachDuringLongPress([](void* ctx) {
			ButtonBinding* b = static_cast<ButtonBinding*>(ctx);
			if (b && b->mgr) {
				b->mgr->dispatchEvent(b->id, ButtonEvent::PRESS_START);
			}
		}, &binding);
	}
}

void ButtonManager::tick() {
	for (ButtonBinding& binding : m_buttons) {
		binding.button.tick();
	}
}

void ButtonManager::dispatchEvent(ButtonId id, ButtonEvent event) {
	// check system handlers first
	for (auto& handler : m_systemHandlers) {
		if (handler(id, event)) {
			return; // vent was consumed by system (e.g. ServiceCLI), STOP here!
		}
	}

	// if no system handler consumed it, pass it to the active game
	if (m_activeCallback) {
		m_activeCallback(id, event);
	}
}
} // namespcae pixelino::driver