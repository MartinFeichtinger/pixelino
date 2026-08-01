#include "app/app_manager.hpp"

#include "core/types.hpp"
#include "core/error_types.hpp"
#include "core/error_handler.hpp"
#include "core/error_observer.hpp"
#include "core/system_logger.hpp"

#include "driver/display.hpp"
#include "driver/button_manager.hpp"

#include "app/serviceCLI.hpp"

namespace pixelino::app {

driver::ButtonManager buttonManager;

void AppManager::begin() {
    // system inits
    core::ErrorHandler::getInstance().setObserver(this);

	// hardware inits
	driver::Display::getInstance().begin();
	buttonManager.begin();

	// service inits
	app::ServiceCLI::getInstance().begin(buttonManager);
    
    // log setup finish
	pixelino::core::SystemLogger::getInstance().logEvent(pixelino::core::LogSource::SYSTEM, "SETUP FINISHED");
}

void AppManager::tick() {
    buttonManager.tick();
    pixelino::app::ServiceCLI::getInstance().tick(); 
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