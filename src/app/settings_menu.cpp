#include "app/settings_menu.hpp"
#include "core/system_logger.hpp"
#include "driver/display.hpp"
#include "app/app_registry.hpp"

namespace pixelino::app {

static bool isSettingsMenuRegistered = []() {
    app::AppRegistry::getInstance().registerApp(
        "SettingsMenu",
        &SettingsMenu::drawIcon,
        []() -> app::IApplication* {
            return new SettingsMenu();
        }
    );
    return true;
}();

void SettingsMenu::onStart() {
    core::SystemLogger::getInstance().logEvent(core::LogSource::SYSTEM, "SETTINGS MENU STARTED");
}

void SettingsMenu::onStop() {
    core::SystemLogger::getInstance().logEvent(core::LogSource::SYSTEM, "SETTINGS MENU STOPPED");
}

void SettingsMenu::draw() {
    driver::Display::getInstance().clear();
    driver::Display::getInstance().show();
}

void SettingsMenu::tick(float deltaTime) {
    // menu logic or animations if needed
}

void SettingsMenu::onButtonEvent(driver::ButtonId id, driver::ButtonEvent event) {
    // menu logic
}

void SettingsMenu::drawIcon() {
    driver::Display& display = driver::Display::getInstance();

    const core::Color _O_ = core::Color(0, 0, 0);          // Transparent / Background (LED off)
    const core::Color GRY = core::Color(200, 200, 200);    // Silver/Grey for the Gear

    static const core::Color iconPixels[64] = {
        _O_, GRY, GRY, _O_, _O_, GRY, GRY, _O_,
        _O_, GRY, GRY, GRY, GRY, GRY, GRY, _O_,
        GRY, GRY, GRY, _O_, _O_, GRY, GRY, GRY,
        GRY, GRY, _O_, _O_, _O_, _O_, GRY, GRY,
        GRY, GRY, _O_, _O_, _O_, _O_, GRY, GRY,
        GRY, GRY, GRY, _O_, _O_, GRY, GRY, GRY,
        _O_, GRY, GRY, GRY, GRY, GRY, GRY, _O_,
        _O_, GRY, GRY, _O_, _O_, GRY, GRY, _O_
    };

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            
            int index = (y * 8) + x;
            core::Color pixelColor = iconPixels[index];

            if (pixelColor.r != 0 || pixelColor.g != 0 || pixelColor.b != 0) {
                display.setPixel(x, y, pixelColor);
            }
        }
    }
}

} // namespace pixelino::app