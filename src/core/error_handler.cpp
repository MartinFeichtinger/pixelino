#include "core/error_handler.hpp"
#include "core/error_types.hpp"

#include "driver/display.hpp"

namespace pixelino::core {

void ErrorHandler::handle(ErrorCode code) {
    if (code == ErrorCode::NONE || m_mode == ErrorMode::SILENT) {
        return;
    }

    if (m_mode >= ErrorMode::LOG_ONLY) {
        // save to 1-byte ring buffer
        m_log[m_head] = code;
        m_head = (m_head + 1) % config::error::max_log_entries;
        if (m_count < config::error::max_log_entries) {
            m_count++;
        }
    }
    
    ErrorLevel level = getErrorLevel(code);

    if ((m_mode == ErrorMode::LOG_LIVE && m_liveOutputEnabled) || m_mode >= ErrorMode::BRODCAST) {

        Serial.print("[");
        Serial.print(levelToString(level));
        Serial.print("] ");
        Serial.println(getErrorMessage(code));
    }

    if ((level == ErrorLevel::FATAL && m_mode == ErrorMode::CRASH_ON_FATAL)
        || (level == ErrorLevel::ERROR && m_mode == ErrorMode::CRASH_ON_ERROR)) {
		driver::Display::getInstance().fill(core::Color::Blue());
		driver::Display::getInstance().show();
        haltSystem();
    }
}

void ErrorHandler::printLogHistory() const {
    if (m_count == 0) {
        Serial.println("error log is empty");
        return;
    }

    size_t startIdx = (m_count == config::error::max_log_entries) ? m_head : 0;

    for (size_t i = 0; i < m_count; i++) {
        size_t index = (startIdx + i) % config::error::max_log_entries;
        ErrorCode code = m_log[index];

        Serial.print(i + 1);
        Serial.print(": [");
        Serial.print(levelToString(getErrorLevel(code)));
        Serial.print("] ");
        Serial.println(getErrorMessage(code));
    }
}

void ErrorHandler::clear() {
    m_head = 0;
    m_count = 0;
    Serial.println("error log cleared");
}

void ErrorHandler::haltSystem() {
    Serial.println("\n*** FATAL ERROR: SYSTEM HALTED ***");
    while (true) {
        delay(100);
    }
}

} // namespace pixelino::core