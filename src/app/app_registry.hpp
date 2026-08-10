#pragma once

#include "app/app_interface.hpp"
#include <vector>
#include <functional>

namespace pixelino::app {

using AppFactory = std::function<IApplication*()>;          // recipe to create a game/app instance
using IconDrawCallback = std::function<void()>;             // callback to render an icon

struct AppRecord {
    const char* name;
    IconDrawCallback drawIcon; 
    AppFactory create;         
};

class AppRegistry {
public:
    static AppRegistry& getInstance() { static AppRegistry instance; return instance; }
    AppRegistry(const AppRegistry&) = delete;
    void operator=(const AppRegistry&) = delete;

    void registerApp(const char* name, IconDrawCallback drawIcon, AppFactory factory) {
        m_apps.push_back({name, drawIcon, factory});
    }

    const std::vector<AppRecord>& getApps() const {
        return m_apps;
    }

private:
    AppRegistry() = default;
    std::vector<AppRecord> m_apps;
};

} // namespace pixelino::app