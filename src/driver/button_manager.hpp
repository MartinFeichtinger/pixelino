#pragma once

#include <OneButton.h>
#include <functional>
#include <array>
#include <vector>
#include "button_types.hpp"

namespace pixelino::driver {

// system handler signature: returns true if event was consumed
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

    void addSystemHandler(SystemButtonHandler handler) {
        m_systemHandlers.push_back(handler);
    }

private:
    struct ButtonBinding {
        ButtonId id;
        OneButton button;
        ButtonManager* mgr; // added for the stateless lambda context
    };

    std::array<ButtonBinding, 9> m_buttons;
    std::vector<SystemButtonHandler> m_systemHandlers;
    ButtonCallback m_activeCallback = nullptr;

    void dispatchEvent(ButtonId id, ButtonEvent event);
};

} // namespace pixelino::driver