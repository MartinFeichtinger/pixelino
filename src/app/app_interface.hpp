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

    // constexpr color aliases
    static constexpr Color BLACK  {core::color::black};
    static constexpr Color WHITE  {core::color::white};
    static constexpr Color RED    {core::color::red};
    static constexpr Color GREEN  {core::color::green};
    static constexpr Color BLUE   {core::color::blue};
    static constexpr Color YELLOW {core::color::yellow};
    static constexpr Color ORAGNE {core::color::orange};
    static constexpr Color PURPLE {core::color::purple};
    static constexpr Color CYAN   {core::color::cyan};

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
    static constexpr int DISPLAY_WIDTH = pixelino::core::config::display::width;
    static constexpr int DISPLAY_HEIGHT = pixelino::core::config::display::height;
    static constexpr int NUM_LEDS = pixelino::core::config::display::num_leds;
};

} // namespace pixelino::app