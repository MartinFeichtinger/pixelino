#include "apps/paint/paint.hpp"
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
    // initialize cursor position
    m_cursor.x = 0;
    m_cursor.y = 0;
    
    // start with red hue
    m_currentHue = 0; 
    m_cursor.col = Color::fromHSV(m_currentHue);

    // clear all canvas pixels to black
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            m_canvas[y][x] = BLACK;
        }
    }
}

void PaintGame::onStop() {
    // exit animation
    // save highscore (not implemented yet)
}

// change to absolut time!!!
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
    // bulk copy canvas into display buffer (one call instead of 64 setPixel calls)
    display.loadBuffer(&m_canvas[0][0], NUM_LEDS); // check functionality

    // overlay cursor (either current hue or white flash)
    if (m_cursorVisible) {
        m_cursor.col = Color::fromHSV(m_currentHue);
        display.setPixel(m_cursor);
    }

    display.show();
}

// ===========================================================================================
// INPUT HANDLING
// ===========================================================================================

void PaintGame::onButtonEvent(ButtonId id, ButtonEvent event) {
    if (event == CLICK) {
        // D-Pad navigation with dynamic wrap-around boundaries
        if (id == KEY_UP) {
            m_cursor.y = (m_cursor.y > 0) ? (m_cursor.y - 1) : DISPLAY_HEIGHT;
        }
        else if (id == KEY_DOWN) {
            m_cursor.y = (m_cursor.y < DISPLAY_HEIGHT) ? (m_cursor.y + 1) : 0;
        }
        else if (id == KEY_LEFT) {
            m_cursor.x = (m_cursor.x > 0) ? (m_cursor.x - 1) : DISPLAY_WIDTH - 1;
        }
        else if (id == KEY_RIGHT) {
            m_cursor.x = (m_cursor.x < DISPLAY_WIDTH - 1) ? (m_cursor.x + 1) : 0;
        }
        
        // Key A: set canvas pixel at cursor to current active color
        else if (id == KEY_A) {
            m_canvas[m_cursor.y][m_cursor.x] = m_cursor.col;
        }
        
        // Key B: cycle color hue by 32 units (8 distinct primary steps per 255 wheel)
        else if (id == KEY_B) {
            m_currentHue += 32;
        }
    }
    else if (event == LONG_PRESS) {
        // set pixel back to black
        if (id == KEY_A) {
            m_canvas[m_cursor.y][m_cursor.x] = BLACK;
        }
    }

	m_blinkTimer=0;
}

// ===========================================================================================
// MENU ICON
// ===========================================================================================

void PaintGame::drawIcon() {
	static const Color iconPixels[NUM_LEDS] = {
		0xFF0000, 0xAB5500, 0xABAA00, 0x00FF00, 0x00AB55, 0x0000FF, 0x5500AB, 0xAA0055, 
		0xAA0055, 0xFF0000, 0xAB5500, 0xABAA00, 0x00FF00, 0x00AB55, 0x0000FF, 0x5500AB, 
		0x5500AB, 0xAA0055, 0xFF0000, 0xAB5500, 0xABAA00, 0x00FF00, 0x00AB55, 0x0000FF, 
		0x0000FF, 0x5500AB, 0xAA0055, 0xFF0000, 0xAB5500, 0xABAA00, 0x00FF00, 0x00AB55, 
		0x00AB55, 0x0000FF, 0x5500AB, 0xAA0055, 0xFF0000, 0xAB5500, 0xABAA00, 0x00FF00, 
		0x00FF00, 0x00AB55, 0x0000FF, 0x5500AB, 0xAA0055, 0xFF0000, 0xAB5500, 0xABAA00, 
		0xABAA00, 0x00FF00, 0x00AB55, 0x0000FF, 0x5500AB, 0xAA0055, 0xFF0000, 0xAB5500, 
		0xAB5500, 0xABAA00, 0x00FF00, 0x00AB55, 0x0000FF, 0x5500AB, 0xAA0055, 0xFF0000
	};

    driver::Display::getInstance().loadBuffer(iconPixels, NUM_LEDS);
}

} // namespace pixelino::apps::paint