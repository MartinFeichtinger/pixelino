/*
#include "apps/empty_template/empty_template.hpp"
#include "app/app_registry.hpp"

namespace pixelino::apps::empty_template {

// =============================================================================================
// SELF-REGISTRATION
// =============================================================================================
static bool isEmptyTemplateRegistered = []() {
    // 8x8 menu icon pixel data
    static const core::Color iconPixels[core::config::display::num_leds] = {
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000
    };

    app::AppRegistry::getInstance().registerApp(
        "EmptyTemplate",
        iconPixels,
        []() -> app::IApplication* {
            return new EmptyTemplate();
        }
    );
    return true;
}();

// ===========================================================================================
// LIFECYCLE METHODS
// ===========================================================================================

void EmptyTemplate::onStart() {
    // called when launching the app
}

void EmptyTemplate::onStop() {
    // called when exiting the app
}

void EmptyTemplate::tick(uint32_t time_ms) {
    // game logic update loop
}

void EmptyTemplate::draw() {
    // rendering logic
}

// ===========================================================================================
// INPUT HANDLING
// ===========================================================================================

void EmptyTemplate::onButtonEvent(ButtonId id, ButtonEvent event) {
    // button input logic
}

} // namespace pixelino::apps::EmptyTemplate
*/