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
    cursor.x = 0;
    cursor.y = 0;
    
    // start with red hue
    currentHue = 0; 
    cursor.col = Color::fromHSV(currentHue);

    // clear all canvas pixels to black
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            canvas[y][x] = BLACK;
        }
    }
}

void PaintGame::onStop() {
    // exit animation
    // save highscore (not implemented yet)
}

void PaintGame::tick(uint32_t time_ms) {
    // cursor blinking 
    if (time_ms - previousCursorBlinkTime_ms < cursorBlinkPeriod_ms/2) {
        isCursorVisible = true;
    }
    else if (time_ms - previousCursorBlinkTime_ms < cursorBlinkPeriod_ms) {
        isCursorVisible = false;
    }
    else {
        previousCursorBlinkTime_ms = time_ms;
    }
}

void PaintGame::draw() {
    // bulk copy canvas into display buffer (one call instead of 64 setPixel calls)
    display.loadBuffer(&canvas[0][0], NUM_LEDS); // check functionality

    // overlay cursor (either current hue or white flash)
    if (isCursorVisible) {
        cursor.col = Color::fromHSV(currentHue);
        display.setPixel(cursor);
    }

    display.show();
}

// ===========================================================================================
// INPUT HANDLING
// ===========================================================================================

void PaintGame::onButtonEvent(ButtonId id, ButtonEvent event) {
    if (event == PRESS) {
        // D-Pad navigation with dynamic wrap-around boundaries
        if (id == KEY_UP) {
            cursor.y = (cursor.y > 0) ? (cursor.y - 1) : DISPLAY_HEIGHT - 1;
        }
        else if (id == KEY_DOWN) {
            cursor.y = (cursor.y < DISPLAY_HEIGHT - 1) ? (cursor.y + 1) : 0;
        }
        else if (id == KEY_LEFT) {
            cursor.x = (cursor.x > 0) ? (cursor.x - 1) : DISPLAY_WIDTH - 1;
        }
        else if (id == KEY_RIGHT) {
            cursor.x = (cursor.x < DISPLAY_WIDTH - 1) ? (cursor.x + 1) : 0;
        }
        
        // Key A: set canvas pixel at cursor to current active color
        else if (id == KEY_A) {
            canvas[cursor.y][cursor.x] = cursor.col;
        }
        
        // Key B: cycle color hue by 32 units (8 distinct primary steps per 255 wheel)
        else if (id == KEY_B) {
            currentHue += 32;
        }
    }
    else if (event == LONG_PRESS) {
        // set pixel back to black
        if (id == KEY_A) {
            canvas[cursor.y][cursor.x] = BLACK;
        }
    }
    // reset cursor blink time to so that the cursor is always visible while he gets manipulated via button presses
    resetCursorBlinkTime();
}

void PaintGame::resetCursorBlinkTime(void) {
    // the cursor blink time gets reset to zero and in the next tick call reset to time_ms
    // when the timing logic in the tick function gets changed this one needs to be adaped too
    previousCursorBlinkTime_ms = 0;
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