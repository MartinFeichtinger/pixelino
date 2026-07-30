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

    // configuration
    void setMode(ErrorMode mode) { m_mode = mode; }
	ErrorMode getMode() {return m_mode;}

    // actual handler
    void handle(ErrorCode code);

private:
    ErrorHandler() = default;

    ErrorMode m_mode = ErrorMode::LOG_LIVE;

    void haltSystem();
};

} // namespace pixelino::core