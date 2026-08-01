#include "core/error_handler.hpp"
#include "core/error_types.hpp"
#include "core/error_observer.hpp"
#include "core/system_logger.hpp"

namespace pixelino::core {

void ErrorHandler::handle(ErrorCode code) {
    if (code == ErrorCode::NONE || m_mode == ErrorMode::SILENT) {
        return;
    }

    ErrorLevel level = getErrorLevel(code);

    if (m_mode >= ErrorMode::LOG_ONLY) {
        SystemLogger::getInstance().logError(code);
    }

    if (m_observer) { m_observer->onError(code, m_mode); }
    
    if ((level == ErrorLevel::FATAL && m_mode == ErrorMode::CRASH_ON_FATAL)
        || (level == ErrorLevel::ERROR && m_mode == ErrorMode::CRASH_ON_ERROR)
        || (level == ErrorLevel::WARNING && m_mode == ErrorMode::CRASH_ON_WARNING)) {
        if (m_observer) { m_observer->onSystemHalt(); }
        haltSystem();
    }
}

void ErrorHandler::setMode(ErrorMode mode) {
    m_mode = mode;
    SystemLogger::getInstance().logEvent(LogSource::SYSTEM, modeToString(mode));
}

void ErrorHandler::haltSystem() {
    while (true) {}
}

} // namespace pixelino::core