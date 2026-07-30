#include "driver/button_manager.hpp"
#include "core/config.hpp"
#include "core/system_logger.hpp"
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

		// long press event (stateless lambda!)
		binding.button.attachDuringLongPress([](void* ctx) {
			ButtonBinding* b = static_cast<ButtonBinding*>(ctx);
			if (b && b->mgr) {
				b->mgr->dispatchEvent(b->id, ButtonEvent::LONG_PRESS);
			}
		}, &binding);
	}
	// add button observer for logging
	addSystemObserver([](driver::ButtonId id, driver::ButtonEvent event) {
		core::SystemLogger::getInstance().logButtonEvent(id, event);
	});
}

void ButtonManager::tick() {
	for (ButtonBinding& binding : m_buttons) {
		binding.button.tick();
	}
}

void ButtonManager::addSystemHandler(SystemButtonHandler handler, HandlerPriority priority) {
    // store as struct containing priority + handler function
    m_systemHandlers.push_back({ priority, handler });

    // sort handlers: Highest priority first
    std::sort(m_systemHandlers.begin(), m_systemHandlers.end(), 
        [](const SystemHandlerEntry& a, const SystemHandlerEntry& b) {
            return static_cast<int>(a.priority) > static_cast<int>(b.priority);
        });
}

void ButtonManager::dispatchEvent(ButtonId id, ButtonEvent event) {
    // PHASE 1: Notify all passive observers (e.g. Audio click sound, Logging)
    // These run regardless of priorities and CANNOT be blocked
    for (const SystemButtonObserver& observer : m_systemObserver) {
        observer(id, event);
    }

    // PHASE 2: Check priority-based interceptors (e.g. ServiceCLI, Menus)
    for (const SystemHandlerEntry& entry : m_systemHandlers) {
        if (entry.handler(id, event)) {
            return; // Consumed by a higher-priority interceptor!
        }
    }

    // PHASE 3: Pass to active game if no interceptor consumed it
    if (m_activeCallback) {
        m_activeCallback(id, event);
    }
}

} // namespcae pixelino::driver