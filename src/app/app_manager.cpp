#include "app/app_manager.hpp"

#include "core/types.hpp"
#include "core/error_types.hpp"
#include "core/error_handler.hpp"
#include "core/error_observer.hpp"
#include "core/system_logger.hpp"

#include "driver/display.hpp"
#include "driver/button_manager.hpp"

#include "app/service_cli.hpp"

namespace pixelino::app {

void AppManager::begin() {
    // system inits
    core::ErrorHandler::getInstance().setObserver(this);

	// hardware inits
	driver::Display::getInstance().begin();
	driver::ButtonManager::getInstance().begin();

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
	pixelino::core::SystemLogger::getInstance().logEvent(pixelino::core::LogSource::SYSTEM, "SETUP FINISHED");
}

void AppManager::tick() {
    driver::ButtonManager::getInstance().tick();
    app::ServiceCLI::getInstance().tick(); 
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