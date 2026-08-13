#pragma once

#include "app/app_interface.hpp"
#include "app/app_registry.hpp"

namespace pixelino::apps::menu {

class MainMenu : public app::IApplication {
public:
    MainMenu() = default;
    ~MainMenu() override = default;

    void onStart() override;
    void onStop() override;
    void tick(float deltaTime) override;
    void draw() override;

    void onButtonEvent(driver::ButtonId id, driver::ButtonEvent event) override;

private:
    static inline int s_selectedIndex = 0;
};

} // namespace pixelino::apps::menu