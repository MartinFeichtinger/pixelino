#pragma once

#include "core/error_types.hpp"
#include "core/error_observer.hpp"

namespace pixelino::core {

class ErrorHandler {
public:
    // sigelton definition
    static ErrorHandler& getInstance() { static ErrorHandler instance; return instance; }
    ErrorHandler(const ErrorHandler&) = delete;
    void operator=(const ErrorHandler&) = delete;

    void handle(ErrorCode code);

    ErrorMode getMode() {return m_mode;}
    void setMode(ErrorMode mode);
    void setObserver(IErrorObserver* observer) { m_observer = observer; }

private:
    ErrorHandler() = default;

    ErrorMode m_mode = ErrorMode::LOG_LIVE;
    IErrorObserver* m_observer = nullptr;

    void haltSystem();
};

} // namespace pixelino::core