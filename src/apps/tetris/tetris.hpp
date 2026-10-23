/**
 * @brief Tetris
 * 
 */

#pragma once

#include "app/app_interface.hpp"
#include <vector>


namespace pixelino::apps::tetris {

#define N_COLORS		6
#define N_TETROMINOS	7
#define TETRA			4
#define BLINK_INTERVAL	200

class Tetris : public app::IApplication {
public:
	Tetris() = default;
	~Tetris() override = default;

	const std::vector<Color> ColorSet = {RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW};		// set of custom colors for tetrominos

	struct Point{											// we also need negative coordinates
		int8_t x;
		int8_t y;
	};

	const Point Tetrominos[N_TETROMINOS][TETRA] = {			// standard tetrominos
		{{0, 0}, {0, 1}, {1, 0}, {1, 1}},	// O
		{{-1, 0}, {0, 0}, {1, 0}, {2, 0}},	// I
		{{-1, 0}, {0, 0}, {1, 0}, {0, 1}},	// T
		{{-1, 0}, {0, 0}, {1, 0}, {1, 1}},	// J
		{{-1, 0}, {0, 0}, {1, 0}, {-1, 1}},	// L
		{{-1, 1}, {0, 1}, {0, 0}, {1, 0}},	// S
		{{-1, 0}, {0, 0}, {0, 1}, {1, 1}}	// Z
	};

	// lifecycle callbacks from the application interface
	void onStart() override;
	void onStop() override;
	void tick(uint32_t time_ms) override;
	void draw() override;

	// button input callback (gets called for every button event)
	void onButtonEvent(ButtonId id, ButtonEvent event) override;

	// static icon renderer for the main menu (is added via the app registry)
	static void drawIcon();

private:

	void rot(Point tetromino[TETRA], int dir);				// rotate a tetromino
	void placeNext();										// get next tetromino
	Color nextColor();										// slect next color based on existing heap

	Color heap[DISPLAY_HEIGHT][DISPLAY_WIDTH];				// heap of already places tetrominos

	Point tetromino[TETRA];									// current tetromino
	Point currentPos;										// current position
	uint8_t currentType;									// type of current tetromino
	Color currentColor;										// color of current tetromino

	uint32_t lastTime_ms;									// last timestamp an interval was triggered
	uint32_t interval_ms;									// interval time (gets faster the longer you play)

	bool gameover;
	bool pause;
	bool blink;
	uint32_t lastBlinkTime;

	std::vector<size_t> tetrominoPool;						// the next tetromino is randomly selected from this pool
															// when the pool is used up, it is filled again

};

} // namespace pixelino::apps::tetris