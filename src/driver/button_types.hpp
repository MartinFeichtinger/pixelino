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

} // namespace pixelino::driver