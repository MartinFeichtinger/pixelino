#include "core/error_handler.hpp"
#include "core/error_types.hpp"
#include "core/system_logger.hpp"
#include "driver/display.hpp"

namespace pixelino::core {

void ErrorHandler::setMode(ErrorMode mode) {
    m_mode = mode;
    SystemLogger::getInstance().logEvent(LogSource::SYSTEM, modeToString(mode));
}

void ErrorHandler::handle(ErrorCode code) {
    if (code == ErrorCode::NONE || m_mode == ErrorMode::SILENT) {
        return;
    }

    ErrorLevel level = getErrorLevel(code);

    if (m_mode >= ErrorMode::LOG_ONLY) {
        SystemLogger::getInstance().logError(code, m_mode);
    }
    
    if ((level == ErrorLevel::FATAL && m_mode == ErrorMode::CRASH_ON_FATAL)
        || (level == ErrorLevel::ERROR && m_mode == ErrorMode::CRASH_ON_ERROR)
        || (level == ErrorLevel::WARNING && m_mode == ErrorMode::CRASH_ON_WARNING))  {
		driver::Display::getInstance().fill(core::Color::Blue());
		driver::Display::getInstance().show();
        haltSystem();
    }
}

void ErrorHandler::haltSystem() {
    Serial.println("\n*** FATAL ERROR: SYSTEM HALTED ***");

    Serial.println("*** LOG HISTORY ***");
    core::SystemLogger::getInstance().printLogHistory();

    Serial.println("\n*** FATAL ERROR: SYSTEM HALTED ***");

    while (true) {
        delay(100);
    }
}

} // namespace pixelino::core