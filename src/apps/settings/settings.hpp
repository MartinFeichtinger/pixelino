#pragma once

#include "app/app_interface.hpp"

namespace pixelino::app {

class SettingsMenu : public app::IApplication {
public:
    SettingsMenu() = default;
    ~SettingsMenu() override = default;

    void onStart() override;
    void onStop() override;
    void draw() override;
    void tick(uint32_t time_ms) override;
    
    void onButtonEvent(driver::ButtonId id, driver::ButtonEvent event) override;

    static void drawIcon();
};

} // namespace pixelino::app