/* deactived to prevent that EmptyTemplate is added to the real apps

#pragma once

#include "app/app_interface.hpp"

namespace pixelino::apps::empty_template {

class EmptyTemplate : public app::IApplication {
public:
    EmptyTemplate() = default;
    ~EmptyTemplate() override = default;

    // lifecycle fuctions
    void onStart() override;
    void onStop() override;
    void tick(uint32_t time_ms) override;
    void draw() override;

    // input handling callback
    void onButtonEvent(ButtonId id, ButtonEvent event) override;

private:
    // game state variables and helper methods
};

} // namespace pixelino::apps::empty_template
*/