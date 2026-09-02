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

namespace pixelino::apps::paint {

class PaintGame : public app::IApplication {
public:
    PaintGame() = default;
    ~PaintGame() override = default;

    // --- lifecycle callbacks ---
    void onStart() override;
    void onStop() override;
    void tick(uint32_t time) override;
    void draw() override;

    // --- input callback ---
    void onButtonEvent(ButtonId id, ButtonEvent event) override;

    // --- static Icon renderer for the Main Menu ---
    static void drawIcon();

private:
    // display size matrix storing the painted canvas colors
    Color canvas[DISPLAY_HEIGHT][DISPLAY_WIDTH];

    // current cursor coordinates on the grid
    Pixel cursor{};

    // selected drawing color hue (0-255)
    uint8_t currentHue = 0;

    // timer and state for blinking cursor feedback
    const uint32_t cursorBlinkPeriod_ms = 1800;
    uint32_t previousCursorBlinkTime_ms = 0;
    bool isCursorVisible = true;
    void resetCursorBlinkTime(void);
};

} // namespace pixelino::apps::paint