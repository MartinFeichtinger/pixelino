#pragma once

#include "core/types.hpp"
#include "app/app_interface.hpp"
#include <vector>
#include <functional>

namespace pixelino::app {

using AppFactory = std::function<IApplication*()>;          // recipe to create a game/app instance
using IconDrawCallback = std::function<void()>;             // callback to render an icon

struct AppRecord {
    const char* name;
    const core::Color* icon;; 
    AppFactory create;         
};

class AppRegistry {
public:
    static AppRegistry& getInstance() { static AppRegistry instance; return instance; }
    AppRegistry(const AppRegistry&) = delete;
    void operator=(const AppRegistry&) = delete;

    void registerApp(const char* name, const core::Color* icon, AppFactory factory) {
        m_apps.push_back({name, icon, factory});
    }

    const std::vector<AppRecord>& getApps() const {
        return m_apps;
    }

private:
    AppRegistry() = default;
    std::vector<AppRecord> m_apps;
};

} // namespace pixelino::app