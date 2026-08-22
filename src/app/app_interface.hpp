#pragma once

#include "driver/button_manager.hpp"
#include "driver/display.hpp"
#include "core/types.hpp"

namespace pixelino::app {

class IApplication {
public:
    virtual ~IApplication() = default;

    virtual void onStart() = 0;
    virtual void onStop() = 0;
    virtual void tick(float deltaTime) = 0;
    virtual void draw() = 0;

    virtual void onButtonEvent(driver::ButtonId id, driver::ButtonEvent event) = 0;

protected:
    // every app automatically gets these ready-to-use references
    driver::Display& display = driver::Display::getInstance();

public:
    // type aliases: creates short names for requiered structs/classes
    using Position = core::Position;
    using Color = core::Color;
    using Pixel = core::Pixel;
    using ButtonId = driver::ButtonId;
    using ButtonEvent = driver::ButtonEvent;

    // constexpr for the enum aliases
    static constexpr ButtonEvent PRESS = ButtonEvent::PRESS;
    static constexpr ButtonEvent CLICK = ButtonEvent::CLICK;
    static constexpr ButtonEvent LONG_PRESS = ButtonEvent::LONG_PRESS;

    static constexpr ButtonId KEY_UP = ButtonId::KEY_UP;
    static constexpr ButtonId KEY_DOWN = ButtonId::KEY_DOWN;
    static constexpr ButtonId KEY_LEFT = ButtonId::KEY_LEFT;
    static constexpr ButtonId KEY_RIGHT = ButtonId::KEY_RIGHT;
    static constexpr ButtonId KEY_A = ButtonId::KEY_A;
    static constexpr ButtonId KEY_B = ButtonId::KEY_B;
    static constexpr ButtonId KEY_START = ButtonId::KEY_START;
    static constexpr ButtonId KEY_SELECT = ButtonId::KEY_SELECT;

    // constants aliases for easier access
    static constexpr int DisplayWidth = pixelino::core::config::display::width;
    static constexpr int DisplayHeight = pixelino::core::config::display::height;
    static constexpr int NumLeds = pixelino::core::config::display::num_leds;
};

} // namespace pixelino::app