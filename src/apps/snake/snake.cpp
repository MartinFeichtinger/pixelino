#include "apps/snake/snake.hpp"
#include "app/app_registry.hpp"
#include <cstdlib>   // for rand()

namespace pixelino::apps::snake {

// ===========================================================================================
// SELF-REGISTRATION
// ===========================================================================================
static bool isSnakeRegistered = []() {
    app::AppRegistry::getInstance().registerApp(
        "Snake",
        &SnakeGame::drawIcon,
        []() -> app::IApplication* {
            return new SnakeGame();
        }
    );
    return true;
}();

// ===========================================================================================
// LIFECYCLE METHODS
// ===========================================================================================

void SnakeGame::onStart() {
    resetGame();
}

void SnakeGame::onStop() {
    // maybe highscore saving
}

void SnakeGame::resetGame() {
    // start in the middle going right
    Position startHeadPos {DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2};
    body[0] = startHeadPos;
    body[1].x = startHeadPos.x - 1;
    body[1].y = startHeadPos.y;
    body[2].x = startHeadPos.x - 2;
    body[2].y = startHeadPos.y;  
    headDirection=RIGHT;
    length = 3;

    // reset game states
    lastTickTime_ms = 0;
    lastMoveTime_ms = 0;
    gameOver = false;
    inputProcessed = false;

    // reset death animation variables
    deathAnimIndex = 0;
    lastDeathSegAnimTime_ms = 0;

    spawnApple();
}

void SnakeGame::spawnApple() {
    size_t emptyFields = NUM_LEDS - length;
    
    if (emptyFields > NUM_LEDS) return;

    size_t targetEmptyIndex = rand() % emptyFields;
    size_t emptyCounter = 0;

    // iterated through every possible position on the display
    for (size_t y = 0; y < DISPLAY_HEIGHT; y++) {
        for (size_t x = 0; x < DISPLAY_WIDTH; x++) {
            // check if specific point is occupied by the snake
            bool isOccupied = false;
            for (size_t i = 0; i < length; i++) {
                if(body[i].x == x && body[i].y == y) {
                    isOccupied = true;
                    break;
                }
            }

            // if field is empty -> count it
            if (!isOccupied) {
                if (emptyCounter == targetEmptyIndex) {
                    apple.x = x;
                    apple.y = y;
                    return;
                }
                else {
                    emptyCounter++;
                }
            }
        }
    }
}

void SnakeGame::tick(uint32_t time_ms) {
    if (gameOver) {
        // play death animation as long as not all segments are red yet
        if (deathAnimIndex < length) {
            if (time_ms - lastDeathSegAnimTime_ms >= 100) { // 0.1 seconds per snake segment
                lastDeathSegAnimTime_ms = time_ms;
                deathAnimIndex++;
            }
        }
        return; // skip normal movement while dead
    }

    if (time_ms - lastMoveTime_ms >= moveInterval_ms) {
        lastMoveTime_ms = time_ms;
        inputProcessed = false; // allow a new button press for the next move

        // calculate next head position
        Position currentHeadPos = body[0];
        Position nextHeadPos;

        switch (headDirection) {
            case RIGHT: {
                nextHeadPos.x = currentHeadPos.x < DISPLAY_WIDTH - 1 ? currentHeadPos.x + 1 : 0;
                nextHeadPos.y = currentHeadPos.y;
                break;
            }
            case LEFT: {
                nextHeadPos.x = currentHeadPos.x > 0 ? currentHeadPos.x - 1 : DISPLAY_WIDTH - 1;
                nextHeadPos.y = currentHeadPos.y;
                break;
            }
            case DOWN: {
                nextHeadPos.x = currentHeadPos.x;
                nextHeadPos.y = currentHeadPos.y < DISPLAY_HEIGHT - 1 ? currentHeadPos.y + 1 : 0;
                break;
            }
            case UP: {
                nextHeadPos.x = currentHeadPos.x;
                nextHeadPos.y = currentHeadPos.y > 0 ? currentHeadPos.y - 1 : DISPLAY_HEIGHT - 1;
                break;
            }
        }

        // check self-collision (don't check the very last tail segment because it moves out of the way)
        for (size_t i = 0; i < length - 1; i++) {
            if (nextHeadPos.x == body[i].x && nextHeadPos.y == body[i].y) {
                gameOver = true;
                // color the head red immediately and start the animation timer
                deathAnimIndex = 1;
                lastDeathSegAnimTime_ms = time_ms;
                return;
            }
        }

        // did the snake ate the apple?
        bool ateApple = (nextHeadPos.x == apple.x && nextHeadPos.y == apple.y);

        if (ateApple) {
            if (length < NUM_LEDS) length++;
        }

        // Shift body array forward
        for (size_t i = length - 1; i > 0; --i) {
            body[i] = body[i - 1];
        }

        // put head in new position
        body[0] = nextHeadPos;

        if (ateApple) {
            spawnApple();
        }
    }

    lastTickTime_ms = time_ms;
}

void SnakeGame::draw() {
    display.fill(BLACK);

    // draw the purple apple
    display.setPixel(apple.x, apple.y, PURPLE);

    // draw the snake
    for (size_t i = 0; i < length; i++) {
        if (gameOver && i < deathAnimIndex) {
            // death animation: pixel turns blood red
            if (i == 0) {
                // bright red for the death head
                display.setPixel(body[i].x, body[i].y, RED);
            }
            else {
                // dark red for the death body
                display.setPixel(body[i].x, body[i].y, Color{20, 0, 0});
            }
        } else {
            // alive: normal green
            if (i == 0) {
                // bright green for the head
                display.setPixel(body[i].x, body[i].y, GREEN);
            } else {
                // slightly darker green for the body
                display.setPixel(body[i].x, body[i].y, Color{0, 100, 0});
            }
        }
    }

    display.show();
}

// ===========================================================================================
// INPUT HANDLING
// ===========================================================================================

void SnakeGame::onButtonEvent(ButtonId id, ButtonEvent event) {
    if (event == PRESS) {

        if (gameOver) {
            // pressing A or B restarts the game if we lost
            if (id == KEY_A || id == KEY_B) {
                resetGame();
            }
            return;
        }

        // only allow one directional change per tick to prevent the snake from
        // turning 180 degrees onto itself in a single frame.
        if (!inputProcessed) {
            if (id == KEY_RIGHT && headDirection != LEFT) {
                headDirection = RIGHT;
                inputProcessed = true;
            }
            else if (id == KEY_LEFT && headDirection != RIGHT) {
                headDirection = LEFT;
                inputProcessed = true;
            }
            else if (id == KEY_UP && headDirection != DOWN) {
                headDirection = UP;
                inputProcessed = true;
            }
            else if (id == KEY_DOWN && headDirection != UP) {
                headDirection = DOWN;
                inputProcessed = true;
            }
        }
    }
}

// ===========================================================================================
// MENU ICON
// ===========================================================================================

void SnakeGame::drawIcon() {
    // custom icon showing a little green snake and a purple apple (0xFF00FF)
    static const Color iconPixels[NUM_LEDS] = {
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
        0x000000, 0x000000, 0x000000, 0x000000, 0x00FF00, 0x00FF00, 0x000000, 0x000000, 
        0x000000, 0x000000, 0x000000, 0x000000, 0x00FF00, 0x000000, 0xFF00FF, 0x000000, 
        0x000000, 0x00FF00, 0x000000, 0x000000, 0x00FF00, 0x000000, 0x000000, 0x000000, 
        0x000000, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0x000000, 0x000000, 0x000000, 
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 
        0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000
    };

    // drawIcon() is static, so it can't use the inherited instance member
    // `display` — fall back to the singleton directly here.
    driver::Display::getInstance().loadBuffer(iconPixels);
}

} // namespace pixelino::apps::snake