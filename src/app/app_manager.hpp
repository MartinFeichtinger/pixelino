#include "core/error_observer.hpp"

namespace pixelino::app {

class AppManager : public core::IErrorObserver {
public:
    static AppManager& getInstance() { static AppManager instance; return instance; }
    AppManager(const AppManager&) = delete;
    void operator=(const AppManager&) = delete;

    void begin();
    void tick();

    // error observer functions
    void onError(core::ErrorCode code, core::ErrorMode mode) override;
    void onSystemHalt() override;

private:
    AppManager() = default;

};

} // namespace pixelino::app