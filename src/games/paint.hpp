/**
 * @brief PaintGame - Template Application
 * 
 * Demonstrates:
 * - Direct screen pixel manipulation via canvas buffer
 * - Cursor navigation with wrap-around movement
 * - Color manipulation using HSV space
 * - Self-registration into AppRegistry
 */

#pragma once

#include "app/app_interface.hpp"
#include "core/config.hpp"  // for display size
#include "core/types.hpp"   // for color and position

namespace pixelino::apps::paint {

class PaintGame : public app::IApplication {
public:
    PaintGame() = default;
    ~PaintGame() override = default;

    // --- lifecycle callbacks ---
    void onStart() override;
    void onStop() override;
    void tick(float deltaTime) override;
    void draw() override;

    // --- input callback ---
    void onButtonEvent(driver::ButtonId id, driver::ButtonEvent event) override;

    // --- static Icon renderer for the Main Menu ---
    static void drawIcon();

private:
    // display size matrix storing the painted canvas colors
    core::Color m_canvas[core::config::display::width][core::config::display::height];

    // current cursor coordinates on the grid
    core::Position m_cursor{};

    // selected drawing color hue (0-255)
    uint8_t m_currentHue = 0;

    // timer and state for blinking cursor feedback
    float m_blinkTimer = 0.0f;
    bool m_cursorVisible = true;
};

} // namespace pixelino::apps::paint