/**
 * @brief SnakeGame - Classic Arcade Game
 * 
 * Demonstrates:
 * - Game loops with timing (tick-based movement)
 * - driver::ButtonEvent::PRESS for immediate inputs
 * - Basic collision detection and wrap-around
 */

#pragma once

#include "app/app_interface.hpp"
#include "core/config.hpp"  
#include "core/types.hpp"   

namespace pixelino::apps::snake {

class SnakeGame : public app::IApplication {
public:
    SnakeGame() = default;
    ~SnakeGame() override = default;

    // --- lifecycle callbacks ---
    void onStart() override;
    void onStop() override;
    void tick(std::uint32_t time_ms) override;
    void draw() override;

    // --- input callback ---
    void onButtonEvent(driver::ButtonId id, driver::ButtonEvent event) override;

    // --- static Icon renderer for the Main Menu ---
    static void drawIcon();

private:
    void resetGame();
    void spawnApple();

    // The maximum size the snake can get is the total number of pixels on screen
    static constexpr int MAX_LENGTH = core::config::display::width * core::config::display::height;
    
    // Game state arrays and variables
    core::Position m_body[MAX_LENGTH];
    int m_length = 3;

    core::Position m_apple{};
    
    // Movement direction (-1, 0, or 1)
    int m_dx = 1;
    int m_dy = 0;

    // Timing and states
    float m_moveTimer = 0.0f;
    float m_moveInterval = 0.35f; // Starts at 0.35 seconds per move
    bool m_gameOver = false;
    
    // Prevents pressing two directions quickly before the next tick executes
    bool m_inputProcessed = false; 

    // Death animaiton variables
    int m_deathAnimIndex = 0;
    float m_deathAnimTimer = 0.0f;
};

} // namespace pixelino::apps::snake