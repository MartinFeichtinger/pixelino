#include "app/app_manager.hpp"

#include "core/types.hpp"
#include "core/error_types.hpp"
#include "core/error_handler.hpp"
#include "core/error_observer.hpp"
#include "core/system_logger.hpp"

#include "driver/display.hpp"
#include "driver/button_manager.hpp"
#include "driver/audio.hpp"

#include "app/service_cli.hpp"
#include "app/app_interface.hpp"
#include "apps/main_menu.hpp"

namespace pixelino::app {

void AppManager::begin() {
    // system inits
    core::ErrorHandler::getInstance().setObserver(this);

	// hardware inits
	driver::Display::getInstance().begin();
	driver::ButtonManager::getInstance().begin();
    driver::Audio::getInstance().begin();

	// service inits
	app::ServiceCLI::getInstance().begin();

    // wire: onboard button click -> toggle ServiceCLI isActive
    driver::ButtonManager::getInstance().addSystemHandler(
        [](driver::ButtonId btn, driver::ButtonEvent evt) {
            if (btn == driver::ButtonId::ONBOARD && evt == driver::ButtonEvent::CLICK) {
                app::ServiceCLI::getInstance().toggle();
            }
			if (btn == driver::ButtonId::ONBOARD) return true;	// we consum all events for this button
			else return false; 									// not our button, let it pass through
        },
        driver::HandlerPriority::HIGH_PRIORITY
    );

    // wire: start key long press -> hard game exit
    driver::ButtonManager::getInstance().addSystemHandler(
        [this](driver::ButtonId btn, driver::ButtonEvent evt) {
            if (btn == driver::ButtonId::KEY_START && evt == driver::ButtonEvent::LONG_PRESS) {
                // force load a fresh main menu
                // switchApp will automatically delete the currently running game
                this->switchApp(new apps::menu::MainMenu());
                return true;    // consume event
            }
            return false;
        },
        driver::HandlerPriority::HIGH_PRIORITY
    );

    // wire: button events observer -> SystemLogger
    driver::ButtonManager::getInstance().addSystemObserver(
        [](driver::ButtonId id, driver::ButtonEvent event) {
            core::SystemLogger::getInstance().logEvent(
                core::LogSource::DRIVER,
                "BUTTON",
                driver::ButtonManager::getInstance().buttonEventToMessage(id, event)
            );
        }
    );
    
    // log setup finish
	core::SystemLogger::getInstance().logEvent(core::LogSource::SYSTEM, "SETUP FINISHED");

    // boot directly in the main menu
    switchApp(new apps::menu::MainMenu());
}

void AppManager::tick() {
    driver::ButtonManager::getInstance().tick();
    driver::ButtonManager::getInstance().tick();
    app::ServiceCLI::getInstance().tick();

    static uint32_t lastFrameTimeMs = 0;
    const uint32_t currentMs = millis();
    const uint32_t frameIntervalMs = 33; // 1000ms / 30fps = 33.3ms

    if (currentMs - lastFrameTimeMs >= frameIntervalMs) {
        // Calculate exact delta time in seconds
        float deltaTime = static_cast<float>(currentMs - lastFrameTimeMs) / 1000.0f;
        lastFrameTimeMs = currentMs;

        if (m_activeApp) {
            m_activeApp->tick(deltaTime);
            m_activeApp->draw();
        }
    }
}

void AppManager::switchApp(IApplication* newApp) {
    // shut down and delete the OLD app to free memory!
    if (m_activeApp) {
        m_activeApp->onStop();  // give it a chance to save dater EEPROM (not implemented yet)
        delete m_activeApp;     // destroy it and free RAM
    }

    // point to the NEW app
    m_activeApp = newApp;

    // start the NEW app
    if (m_activeApp) {
        m_activeApp->onStart();

        // route buttons to the new app
        driver::ButtonManager::getInstance().setActiveCallback(
            [this](driver::ButtonId id, driver::ButtonEvent event) {
                if (m_activeApp) m_activeApp->onButtonEvent(id, event);
            }
        );
    } else {
        driver::ButtonManager::getInstance().clearActiveCallback();
    }
}

void AppManager::onError(core::ErrorCode code, core::ErrorMode mode) {
    // format live error output
    ServiceCLI::getInstance().printLiveError(code, mode);
}

void AppManager::onSystemHalt() {
    // display driver show blue-screen    
    driver::Display::getInstance().fill(core::Color::Blue());
    driver::Display::getInstance().show();

    // print crash report on the serial monitor
    ServiceCLI::getInstance().printSystemCrashMsg();
    
    // stop the active game/app if you have one
    // if (m_activeApp) m_activeApp->stop();
}

} // namespace pixelino::app