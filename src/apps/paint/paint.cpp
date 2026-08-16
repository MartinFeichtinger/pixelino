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

	if (m_blinkTimer <= 0.5f) {
		m_cursorVisible = true;
	}
	else if (m_blinkTimer <= 1.0f) {
		m_cursorVisible = false;
	}
	else if (m_blinkTimer > 1.0f) {
		m_blinkTimer=0;
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
            m_canvas[m_cursor.y][m_cursor.x] = core::Color::Black();
        }
    }

	m_blinkTimer=0;
}

// ===========================================================================================
// MENU ICON
// ===========================================================================================

void PaintGame::drawIcon() {
    driver::Display& display = driver::Display::getInstance();

	static const core::Color iconPixels[core::config::display::num_leds] = {
		0xFF0000, 0xAB5500, 0xABAA00, 0x00FF00, 0x00AB55, 0x0000FF, 0x5500AB, 0xAA0055, 
		0xAA0055, 0xFF0000, 0xAB5500, 0xABAA00, 0x00FF00, 0x00AB55, 0x0000FF, 0x5500AB, 
		0x5500AB, 0xAA0055, 0xFF0000, 0xAB5500, 0xABAA00, 0x00FF00, 0x00AB55, 0x0000FF, 
		0x0000FF, 0x5500AB, 0xAA0055, 0xFF0000, 0xAB5500, 0xABAA00, 0x00FF00, 0x00AB55, 
		0x00AB55, 0x0000FF, 0x5500AB, 0xAA0055, 0xFF0000, 0xAB5500, 0xABAA00, 0x00FF00, 
		0x00FF00, 0x00AB55, 0x0000FF, 0x5500AB, 0xAA0055, 0xFF0000, 0xAB5500, 0xABAA00, 
		0xABAA00, 0x00FF00, 0x00AB55, 0x0000FF, 0x5500AB, 0xAA0055, 0xFF0000, 0xAB5500, 
		0xAB5500, 0xABAA00, 0x00FF00, 0x00AB55, 0x0000FF, 0x5500AB, 0xAA0055, 0xFF0000
	};

    driver::Display::getInstance().loadBuffer(iconPixels, core::config::display::num_leds);
}

} // namespace pixelino::apps::paint