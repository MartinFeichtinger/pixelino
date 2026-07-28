#pragma once

#include <OneButton.h>
#include <functional>
#include <array>
#include <vector>
#include "button_types.hpp"

namespace pixelino::driver {

// system handler signature: returns true if event was consumed
using SystemButtonObserver = std::function<void(ButtonId, ButtonEvent)>;
using SystemButtonHandler = std::function<bool(ButtonId, ButtonEvent)>;
using ButtonCallback = std::function<void(ButtonId, ButtonEvent)>;

class ButtonManager {
public:
    ButtonManager();

    void begin();
    void tick();

    void setActiveCallback(ButtonCallback callback) {
        m_activeCallback = callback;
    }

    void clearActiveCallback() {
        m_activeCallback = nullptr;
    }

    // system observer: allways run, cannot block/consume events
    void addSystemObserver(SystemButtonObserver observer) {
        m_systemObserver.push_back(observer);
    }

    // system handlers: run by priority, CAN consume events
    void addSystemHandler(SystemButtonHandler handler, HandlerPriority priority = HandlerPriority::NORMAL_PRIORITY);

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
};

} // namespace pixelino::driver