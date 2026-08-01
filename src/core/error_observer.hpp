#pragma once
#include "error_types.hpp"

namespace pixelino::core {

class IErrorObserver {
public:
    virtual ~IErrorObserver() = default;

    virtual void onError(ErrorCode code, ErrorMode mode) = 0;
    virtual void onSystemHalt() = 0;
};

} // namespace pixelino::core