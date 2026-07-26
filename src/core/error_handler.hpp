#pragma once

#include <Arduino.h>
#include <cstdint>
#include "core/config.hpp"
#include "core/error_types.hpp"

namespace pixelino::core {

class ErrorHandler {
public:
    static ErrorHandler& getInstance() {
        static ErrorHandler instance;
        return instance;
    }

    ErrorHandler(const ErrorHandler&) = delete;
    void operator=(const ErrorHandler&) = delete;

    // Configuration
    void setMode(ErrorMode mode) { m_mode = mode; }
    void setLiveOutput(bool enabled) { m_liveOutputEnabled = enabled; }
	ErrorMode getMode() {return m_mode;}

    void handle(ErrorCode code);
    void printLogHistory() const;
    void clear();

private:
    ErrorHandler() = default;

    void haltSystem();

    // 1-Byte per entry ring buffer
    ErrorCode m_log[config::error::max_log_entries];
    size_t m_head = 0;
    size_t m_count = 0;

    ErrorMode m_mode = ErrorMode::LOG_LIVE;
    bool m_liveOutputEnabled = false;
};

} // namespace pixelino::core