#include "apps/main_menu.hpp"
#include "app/app_manager.hpp"
#include "driver/display.hpp"
#include "core/system_logger.hpp"

namespace pixelino::apps::menu {

void MainMenu::onStart() {
    core::SystemLogger::getInstance().logEvent(core::LogSource::SYSTEM, "MAIN MENU STARTED");
}

void MainMenu::onStop() {
    core::SystemLogger::getInstance().logEvent(core::LogSource::SYSTEM, "MAIN MENU STOPPED");
}

void MainMenu::tick(float deltaTime) {
    // menu logic or animations if needed
}

void MainMenu::draw() {
    driver::Display& display = driver::Display::getInstance();
    display.clear();

    const std::vector<app::AppRecord>& apps = app::AppRegistry::getInstance().getApps();
    if (apps.empty()) {
        display.show();
        return;
    }

    if (apps[s_selectedIndex].drawIcon) {
        apps[s_selectedIndex].drawIcon();
    }

    display.show();
}

void MainMenu::onButtonEvent(driver::ButtonId id, driver::ButtonEvent event) {
    const std::vector<app::AppRecord>& apps = app::AppRegistry::getInstance().getApps();
    if (apps.empty()) return;

    int totalApps = apps.size();

    if (event == driver::ButtonEvent::CLICK) {
        
        // KEY_UP -> Circular step backward
        if (id == driver::ButtonId::KEY_UP) {
            s_selectedIndex = (s_selectedIndex - 1 + totalApps) % totalApps;
        } 
        
        // KEY_DOWN -> Circular step forward
        else if (id == driver::ButtonId::KEY_DOWN) {
            s_selectedIndex = (s_selectedIndex + 1) % totalApps;
        } 
        
        // KEY_START or KEY_A -> Launch selected game/app
        else if (id == driver::ButtonId::KEY_START) {
            if (apps[s_selectedIndex].create) {
                app::IApplication* newApp = apps[s_selectedIndex].create();
                app::AppManager::getInstance().switchApp(newApp);
            }
        }
    }
}

} // namespace pixelino::apps::menu