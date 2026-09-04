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
    void onButtonEvent(ButtonId id, ButtonEvent event) override;

    // --- static Icon renderer for the Main Menu ---
    static void drawIcon();

private:
    void resetGame();
    void spawnApple();

    enum Direction {UP, DOWN, LEFT, RIGHT};

    // game state arrays and variables
    std::uint8_t length = 3;
    Position body[NUM_LEDS];
    Position apple;
    Direction headDirection;

    // timing and states
    uint32_t lastTickTime_ms = 0;
    uint32_t lastMoveTime_ms = 0;
    const uint32_t moveInterval_ms = 350;
    bool gameOver = false;
    bool inputProcessed = false;        // prevents pressing two directions quickly before the next tick executes

    // death animation variables
    size_t deathAnimIndex = 0;
    uint32_t lastDeathSegAnimTime_ms = 0;
};

} // namespace pixelino::apps::snake