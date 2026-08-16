#include "apps/paint/paint.hpp"
#include "core/system_logger.hpp"
#include "driver/display.hpp"
#include "app/app_registry.hpp"

namespace pixelino::apps::paint {

// ===========================================================================================
// SELF-REGISTRATION
// ===========================================================================================
// runs once at startup before setup(). adds Paint to the Main Menu without 
// needing to include paint_game.hpp inside main_menu.cpp.
static bool isPaintRegistered = []() {
    app::AppRegistry::getInstance().registerApp(
        "Paint",
        &PaintGame::drawIcon,
        []() -> app::IApplication* {
            return new PaintGame();
        }
    );
    return true;
}();

// ===========================================================================================
// LIFECYCLE METHODS
// ===========================================================================================

void PaintGame::onStart() {
    core::SystemLogger::getInstance().logEvent(core::LogSource::SYSTEM, "PAINT STARTED");

    // initialize cursor position
    m_cursor.x = 0;
    m_cursor.y = 0;
    
    // start with red hue
    m_currentHue = 0; 

    // clear all canvas pixels to black
    for (int y = 0; y < core::config::display::height; ++y) {
        for (int x = 0; x < core::config::display::width; ++x) {
            m_canvas[y][x] = core::Color{0, 0, 0};
        }
    }
}

void PaintGame::onStop() {
    core::SystemLogger::getInstance().logEvent(core::LogSource::SYSTEM, "PAINT STOPPED");
}

void PaintGame::tick(float deltaTime) {
    // blink cursor every 0.25 seconds for visual identification
    m_blinkTimer += deltaTime;
    if (m_blinkTimer >= 0.5f) {
        m_blinkTimer = 0.0f;
        m_cursorVisible = !m_cursorVisible;
    }
}

void PaintGame::draw() {
    driver::Display& display = driver::Display::getInstance();
    display.clear();

    // bulk copy canvas into display buffer (one call instead of 64 setPixel calls)
    display.loadBuffer(&m_canvas[0][0], core::config::display::num_leds);

    // overlay cursor (either current hue or white flash)
    if (m_cursorVisible) {
        core::Color cursorColor = core::Color::fromHSV(m_currentHue, 255, 255);
        display.setPixel(m_cursor.x, m_cursor.y, cursorColor);
    }

    display.show();
}

// ===========================================================================================
// INPUT HANDLING
// ===========================================================================================

void PaintGame::onButtonEvent(driver::ButtonId id, driver::ButtonEvent event) {
    if (event == driver::ButtonEvent::CLICK) {
        // cache limits to avoid typing them repeatedly
        std::uint8_t maxX = core::config::display::width - 1;
        std::uint8_t maxY = core::config::display::height - 1;

        // D-Pad navigation with dynamic wrap-around boundaries
        if (id == driver::ButtonId::KEY_UP) {
            m_cursor.y = (m_cursor.y > 0) ? (m_cursor.y - 1) : maxY;
        } 
        else if (id == driver::ButtonId::KEY_DOWN) {
            m_cursor.y = (m_cursor.y < maxY) ? (m_cursor.y + 1) : 0;
        } 
        else if (id == driver::ButtonId::KEY_LEFT) {
            m_cursor.x = (m_cursor.x > 0) ? (m_cursor.x - 1) : maxX;
        } 
        else if (id == driver::ButtonId::KEY_RIGHT) {
            m_cursor.x = (m_cursor.x < maxX) ? (m_cursor.x + 1) : 0;
        }
        
        // Key A: set canvas pixel at cursor to current active color
        else if (id == driver::ButtonId::KEY_A) {
            m_canvas[m_cursor.y][m_cursor.x] = core::Color::fromHSV(m_currentHue, 255, 255);
        }
        
        // Key B: cycle color hue by 32 units (8 distinct primary steps per 255 wheel)
        else if (id == driver::ButtonId::KEY_B) {
            m_currentHue += 32;
        }
    }
    else if (event == driver::ButtonEvent::LONG_PRESS) {
        // set pixel back to black
        if (id == driver::ButtonId::KEY_A) {
            m_canvas[m_cursor.y][m_cursor.x] = core::Color (0,0,0);
        }
    }
}

// ===========================================================================================
// MENU ICON
// ===========================================================================================

void PaintGame::drawIcon() {
    driver::Display& display = driver::Display::getInstance();

    // color aliases to make the 8x8 grid readable in code
    const core::Color _O_ = core::Color(0, 0, 0);          // Transparent / Black
    const core::Color BLU = core::Color(100, 150, 255);    // Palette Light Blue
    const core::Color RED = core::Color(255, 0, 0);        // Red Paint
    const core::Color GRN = core::Color(0, 255, 0);        // Green Paint
    const core::Color YLW = core::Color(255, 165, 0);      // Yellow/Orange Paint
    const core::Color HND = core::Color(200, 100, 0);      // Brush Handle
    const core::Color FER = core::Color(150, 150, 150);    // Brush Metal Ferrule
    const core::Color BRS = core::Color(210, 180, 140);    // Brush Bristles

    // 64-pixel array (8x8) representing a painting pallet
    static const core::Color iconPixels[core::config::display::num_leds] = {
        _O_, _O_, _O_, _O_, _O_, _O_, BRS, BRS,
        _O_, _O_, BLU, BLU, BLU, BRS, FER, _O_,
        _O_, GRN, YLW, BLU, BLU, HND, _O_, _O_,
        RED, BLU, BLU, BLU, HND, BLU, _O_, _O_,
        BLU, BLU, _O_, BLU, HND, _O_, _O_, _O_,
        BLU, BLU, BLU, HND, BLU, _O_, _O_, _O_,
        _O_, BLU, BLU, HND, _O_, _O_, _O_, _O_,
        _O_, _O_, _O_, _O_, _O_, _O_, _O_, _O_
    };

    driver::Display::getInstance().loadBuffer(iconPixels, core::config::display::num_leds);
}

} // namespace pixelino::apps::paint