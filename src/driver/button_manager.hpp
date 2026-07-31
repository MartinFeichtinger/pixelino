#pragma once

#include <OneButton.h>
#include <cstdint>
#include <functional>
#include <array>
#include <vector>

namespace pixelino::driver {

enum class ButtonId {
	KEY_UP = 0,
	KEY_DOWN = 1,
	KEY_LEFT = 2,
	KEY_RIGHT = 3,
	KEY_A = 4,
	KEY_B = 5,
	KEY_SELECT = 6,
	KEY_START = 7,
	ONBOARD = 8
};

enum class ButtonEvent {
	PRESS = 0,
	CLICK = 1,
	LONG_PRESS =2
};

// higher number = higher priority (executes first)
enum class HandlerPriority : int {
	LOW_PRIORITY		= 10,  // e.g. background analytics, logging
	NORMAL_PRIORITY		= 50,  // e.g. passive click sound feedback
	HIGH_PRIORITY		= 100, // e.g. serviceCLI, settings menu
	CRITICAL_PRIORITY	= 200  // e.g. system reset, emergency hardware overrides
};

using SystemButtonObserver = std::function<void(ButtonId, ButtonEvent)>;
using SystemButtonHandler = std::function<bool(ButtonId, ButtonEvent)>;
using ButtonCallback = std::function<void(ButtonId, ButtonEvent)>;

class ButtonManager {
public:
    ButtonManager();

    void begin();
    void tick();

    // system observers: allways run, cannot block/consume events
    void addSystemObserver(SystemButtonObserver observer) { m_systemObserver.push_back(observer); }

    // system handlers: run by priority, CAN consume events
    void addSystemHandler(SystemButtonHandler handler, HandlerPriority priority = HandlerPriority::NORMAL_PRIORITY);

    // callbacks: can be set by the active application (can allready be consumed by system handlers!)
    void setActiveCallback(ButtonCallback callback) { m_activeCallback = callback; }
    void clearActiveCallback() { m_activeCallback = nullptr; }

private:
    struct ButtonBinding {
        ButtonId id;
        OneButton button;
        ButtonManager* mgr; // added for the stateless lambda context
    };

    struct SystemHandlerEntry {
        HandlerPriority priority;
        SystemButtonHandler handler;
    };

    std::array<ButtonBinding, 9> m_buttons;
    std::vector<SystemButtonObserver> m_systemObserver;
    std::vector<SystemHandlerEntry> m_systemHandlers;
    ButtonCallback m_activeCallback = nullptr;

    void dispatchEvent(ButtonId id, ButtonEvent event);

    static const char* buttonEventToMessage(ButtonId id, ButtonEvent event);
};

} // namespace pixelino::driver