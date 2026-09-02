#include "apps/snake/snake.hpp"
#include "core/system_logger.hpp"
#include "driver/display.hpp"
#include "app/app_registry.hpp"
#include <cstdlib>   // for rand()
#include <algorithm> // for std::max

namespace pixelino::apps::snake {

// ===========================================================================================
// SELF-REGISTRATION
// ===========================================================================================
/*static bool isSnakeRegistered = []() {
    app::AppRegistry::getInstance().registerApp(
        "Snake",
        &SnakeGame::drawIcon,
        []() -> app::IApplication* {
            return new SnakeGame();
        }
    );
    return true;
}();*/

// ===========================================================================================
// LIFECYCLE METHODS
// ===========================================================================================

void SnakeGame::onStart() {
    core::SystemLogger::getInstance().logEvent(core::LogSource::SYSTEM, "SNAKE STARTED");
    resetGame();
}

void SnakeGame::onStop() {
    core::SystemLogger::getInstance().logEvent(core::LogSource::SYSTEM, "SNAKE STOPPED");
}

void SnakeGame::resetGame() {
    m_length = 3;
    
    // Start in the middle going right
    uint8_t startX = core::config::display::width / 2;
    uint8_t startY = core::config::display::height / 2;
    
    // Explicitly cast the math results to uint8_t to prevent narrowing warnings
    m_body[0] = {startX, startY};
    m_body[1] = {static_cast<uint8_t>(startX - 1), startY};
    m_body[2] = {static_cast<uint8_t>(startX - 2), startY};

    m_dx = 1;
    m_dy = 0;
    
    m_moveTimer = 0.0f;
    m_moveInterval = 0.35f;
    m_gameOver = false;
    m_inputProcessed = false;

    // Reset death animation variables
    m_deathAnimIndex = 0;
    m_deathAnimTimer = 0.0f;

    spawnApple();
}

void SnakeGame::spawnApple() {
    bool valid = false;
    while (!valid) {
        m_apple.x = rand() % core::config::display::width;
        m_apple.y = rand() % core::config::display::height;
        
        valid = true;
        // Make sure the apple doesn't spawn ON the snake
        for (int i = 0; i < m_length; ++i) {
            if (m_body[i].x == m_apple.x && m_body[i].y == m_apple.y) {
                valid = false;
                break;
            }
        }
    }
}
/*
void SnakeGame::tick(float deltaTime) {
    if (m_gameOver) {
        // Play death animation as long as not all segments are red yet
        if (m_deathAnimIndex < m_length) {
            m_deathAnimTimer += deltaTime;
            if (m_deathAnimTimer >= 0.1f) { // 0.1 seconds per snake segment
                m_deathAnimTimer = 0.0f;
                m_deathAnimIndex++;
            }
        }
        return; // Skip normal movement while dead
    }

    m_moveTimer += deltaTime;

    if (m_moveTimer >= m_moveInterval) {
        m_moveTimer = 0.0f;
        m_inputProcessed = false; // Allow a new button press for the next move

        // Calculate where the head will go next using signed integers
        int nextX = m_body[0].x + m_dx;
        int nextY = m_body[0].y + m_dy;

        // Wrap around boundaries (Snake II style)
        if (nextX < 0) nextX = core::config::display::width - 1;
        else if (nextX >= core::config::display::width) nextX = 0;
        
        if (nextY < 0) nextY = core::config::display::height - 1;
        else if (nextY >= core::config::display::height) nextY = 0;

        // Now that bounds are checked, safely cast to uint8_t
        core::Position nextHead = {static_cast<uint8_t>(nextX), static_cast<uint8_t>(nextY)};

        // Check self-collision (we don't check the very last tail segment 
        // because it will move forward out of the way)
        for (int i = 0; i < m_length - 1; ++i) {
            if (nextHead.x == m_body[i].x && nextHead.y == m_body[i].y) {
                m_gameOver = true;
                // Color the head red immediately and start the animation timer
                m_deathAnimIndex = 1; 
                m_deathAnimTimer = 0.0f;
                return;
            }
        }

        // Did we eat the apple?
        bool ateApple = (nextHead.x == m_apple.x && nextHead.y == m_apple.y);
        
        if (ateApple) {
            if (m_length < MAX_LENGTH) m_length++;
            // Make the game progressively faster
            m_moveInterval = std::max(0.10f, m_moveInterval - 0.015f); 
        }

        // Shift body array forward
        for (int i = m_length - 1; i > 0; --i) {
            m_body[i] = m_body[i - 1];
        }
        
        // Put head in new position
        m_body[0] = nextHead;

        if (ateApple) {
            spawnApple();
        }
    }
}
*/
void SnakeGame::draw() {
    driver::Display& display = driver::Display::getInstance();
    display.clear();

    // Draw the purple apple (Magenta)
    display.setPixel(m_apple.x, m_apple.y, core::Color{255, 0, 255}); 

    // Draw the snake
    for (int i = 0; i < m_length; ++i) {
        if (m_gameOver && i < m_deathAnimIndex) {
            // Death animation: Pixel turns blood red
            if (i == 0) {
                // Bright red for the death head
                display.setPixel(m_body[i].x, m_body[i].y, core::Color{255, 0, 0});
            }
            else {
                // Dark red for the death body
                display.setPixel(m_body[i].x, m_body[i].y, core::Color{20, 0, 0});
            }
        } else {
            // Alive: Normal green
            if (i == 0) {
                // Bright green for the head
                display.setPixel(m_body[i].x, m_body[i].y, core::Color{0, 255, 0});
            } else {
                // Slightly darker green for the body
                display.setPixel(m_body[i].x, m_body[i].y, core::Color{0, 100, 0});
            }
        }
    }

    display.show();
}

// ===========================================================================================
// INPUT HANDLING
// ===========================================================================================

void SnakeGame::onButtonEvent(driver::ButtonId id, driver::ButtonEvent event) {
    // USING PRESS INSTEAD OF CLICK FOR IMMEDIATE REACTION
    if (event == driver::ButtonEvent::PRESS) {
        
        if (m_gameOver) {
            // Pressing A or B restarts the game if we lost
            if (id == driver::ButtonId::KEY_A || id == driver::ButtonId::KEY_B) {
                resetGame();
            }
            return;
        }

        // Only allow one directional change per tick to prevent the snake from 
        // turning 180 degrees onto itself in a single frame.
        if (!m_inputProcessed) {
            if (id == driver::ButtonId::KEY_UP && m_dy != 1) {
                m_dx = 0; m_dy = -1; 
                m_inputProcessed = true;
            } 
            else if (id == driver::ButtonId::KEY_DOWN && m_dy != -1) {
                m_dx = 0; m_dy = 1; 
                m_inputProcessed = true;
            } 
            else if (id == driver::ButtonId::KEY_LEFT && m_dx != 1) {
                m_dx = -1; m_dy = 0; 
                m_inputProcessed = true;
            } 
            else if (id == driver::ButtonId::KEY_RIGHT && m_dx != -1) {
                m_dx = 1; m_dy = 0; 
                m_inputProcessed = true;
            }
        }
    }
}

// ===========================================================================================
// MENU ICON
// ===========================================================================================

void SnakeGame::drawIcon() {
    // Custom icon showing a little green snake and a purple apple (0xFF00FF)
    static const core::Color iconPixels[core::config::display::num_leds] = {
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
        0x000000, 0x000000, 0x000000, 0x000000, 0x00FF00, 0x00FF00, 0x000000, 0x000000, 
        0x000000, 0x000000, 0x000000, 0x000000, 0x00FF00, 0x000000, 0xFF00FF, 0x000000, 
        0x000000, 0x00FF00, 0x000000, 0x000000, 0x00FF00, 0x000000, 0x000000, 0x000000, 
        0x000000, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x000000, 0x000000, 0x000000, 
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000
    };

    driver::Display::getInstance().loadBuffer(iconPixels, core::config::display::num_leds);
}

} // namespace pixelino::apps::snake