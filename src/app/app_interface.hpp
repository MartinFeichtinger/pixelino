#pragma once

#include "driver/button_manager.hpp"

namespace pixelino::app {

class IApplication {
public:
    virtual ~IApplication() = default;

    virtual void onStart() = 0;
    virtual void onStop() = 0;
    virtual void tick(float deltaTime) = 0;
    virtual void draw() = 0;

    virtual void onButtonEvent(driver::ButtonId id, driver::ButtonEvent event) = 0;
};

} // namespace pixelino::app