#include "apps/tetris/tetris.hpp"
#include "app/app_registry.hpp"

#include <vector>


namespace pixelino::apps::tetris {

// ===========================================================================================
// SELF-REGISTRATION
// ===========================================================================================
static bool isTetrisRegistered = []() {
	
	// define some colors
	const core::Color R = core::color::red;
	const core::Color G = core::color::green;
	const core::Color B = core::color::blue;
	const core::Color C = core::color::cyan;
	const core::Color M = core::color::magenta;
	const core::Color Y = core::color::yellow;
	const core::Color K = core::color::black;
	const core::Color W = core::color::white;

	// custom icon that gets shown in the main menu
	static const core::Color iconPixels[core::config::display::num_leds] = {
		K, K, K, K, K, K, K, K,
		K, K, K, R, R, R, K, K,
		K, K, K, R, K, K, K, K,
		K, K, K, K, K, K, K, K,
		K, C, K, K, K, K, K, K,
		C, C, K, Y, K, K, K, K,
		C, M, M, Y, K, K, B, B,
		M, M, Y, Y, K, K, B, B,
	};


	app::AppRegistry::getInstance().registerApp(
		"Tetris",
		iconPixels,
		[]() -> app::IApplication* {
			return new Tetris();
		}
	);
	return true;
}();

// ===========================================================================================
// LIFECYCLE METHODS
// ===========================================================================================

void Tetris::onStart() {

	gameover = false;
	pause = false;
	blink = true;

	// clear heap
	for (int y = 0; y < DISPLAY_HEIGHT; y++) {
		for (int x = 0; x < DISPLAY_WIDTH; x++) {
			heap[y][x] = BLACK;
		}
	}

	lastTime_ms = 0;
	interval_ms = 1000;

	placeNext();
}

void Tetris::onStop() {

}

// place for the non button related game logic
void Tetris::tick(uint32_t time_ms) {

	if(gameover){
		return;
	}

	if(lastBlinkTime == 0){
		lastBlinkTime = time_ms;
	}

	if(time_ms > lastBlinkTime + BLINK_INTERVAL){
		lastBlinkTime += BLINK_INTERVAL;
		blink = !blink;
	}

	// use first tick to set lastTime_ms
	if(lastTime_ms == 0){
		lastTime_ms = time_ms;
		return;
	}

	// interval has passed
	if(time_ms > lastTime_ms + interval_ms){
		lastTime_ms += interval_ms;

		if(pause){
			return;
		}

		// check for white lines
		bool whiteLines = false;
		for(size_t y = 0; y < DISPLAY_HEIGHT; y++){
			if(heap[y][0] == WHITE){
				whiteLines = true;

				// copy lines down
				for(int yy = y - 1; yy >= 0; yy--){
					for(size_t x = 0; x < DISPLAY_WIDTH; x++){
						heap[yy + 1][x] = heap[yy][x];
					}
				}

				// clear first line
				for(size_t x = 0; x < DISPLAY_WIDTH; x++){
					heap[0][x] = BLACK;
				}

				interval_ms *= 0.95;	// for every white line reduce interval by 2%
			}
		}
		if(whiteLines){
			return;
		}


		// copy current position and orientation
		Point testPos = currentPos;
		Point testTetromino[TETRA];

		for(size_t i = 0; i < TETRA; i++){
			testTetromino[i] = tetromino[i];
		}

		// try to lower tetromino and detect collison
		testPos.y++;
		bool touchDown = false;

		for(size_t i = 0; i < TETRA; i++){
			
			// check floor
			if(testTetromino[i].y + testPos.y >= DISPLAY_HEIGHT){
				touchDown = true;
			}

			// check heap
			int8_t x = testTetromino[i].x + testPos.x;
			int8_t y = testTetromino[i].y + testPos.y;

			if( x >= 0 && y >= 0 && x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT){
				if(heap[y][x] != BLACK){
					touchDown = true;
				}
			}
		}

		if(!touchDown){
			// accept movement
			currentPos = testPos;
			for(size_t i = 0; i < TETRA; i++){
				tetromino[i] = testTetromino[i];
			}
		}
		else{
			// touchDown, add tetromino to heap
			for(size_t i = 0; i < TETRA; i++){
				heap[tetromino[i].y + currentPos.y][tetromino[i].x + currentPos.x] = currentColor;
			}

			// check for complete rows
			for(size_t y = 0; y < DISPLAY_HEIGHT; y++){
				bool rowComplete = true;
				for(size_t x = 0; x < DISPLAY_WIDTH; x++){
					if(heap[y][x] == BLACK){
						rowComplete = false;
					}
				}

				// mark complete row white
				if(rowComplete)
				{
					for(size_t x = 0; x < DISPLAY_WIDTH; x++){
						heap[y][x] = WHITE;
					}
				}
			}

			placeNext();

			// check for collision with new tetromino -> game over
			for(size_t i = 0; i < TETRA; i++){

				int8_t x = tetromino[i].x + currentPos.x;
				int8_t y = tetromino[i].y + currentPos.y;

				if( x >= 0 && y >= 0 && x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT){
					if(heap[y][x] != BLACK){
						gameover = true;
					}
				}
			}
		}
	}
}

// place the graphics rendering and display output
void Tetris::draw() {

	Color canvas[DISPLAY_HEIGHT][DISPLAY_WIDTH];

	// draw heap
	for (int y = 0; y < DISPLAY_HEIGHT; y++) {
		for (int x = 0; x < DISPLAY_WIDTH; x++) {
			canvas[y][x] = heap[y][x];
		}
	}

	// draw tetromino
	if(!pause || blink){
		for(size_t i = 0; i < TETRA; i++)
		{
			int8_t x = tetromino[i].x + currentPos.x;
			int8_t y = tetromino[i].y + currentPos.y;

			if( x >= 0 && y >= 0 && x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT)
			{
				canvas[y][x] = currentColor;
			}
		}
	}

	// bulk copy canvas into display buffer (one call instead of 64 setPixel calls)
	display.loadBuffer(&canvas[0][0]); // reference to the first element (this syntax is required in c/c++ for 2D-arrays)

	// actual updating of the display
	display.show();
}

// place for lifecyle helper functions


// ===========================================================================================
// INPUT HANDLING
// ===========================================================================================

// place for the button input handling logic
void Tetris::onButtonEvent(ButtonId id, ButtonEvent event) {
	if(gameover){
		return;
	}

	if (event == PRESS) {

		if (id == KEY_SELECT){
			pause = !pause;
		}

		if(pause){
			return;
		}

		// copy current position and orientation
		Point testPos = currentPos;
		Point testTetromino[TETRA];

		for(size_t i = 0; i < TETRA; i++){
			testTetromino[i] = tetromino[i];
		}

		// test movements
		if (id == KEY_UP){
			rot(testTetromino, 1);
		}
		else if (id == KEY_DOWN) {
			testPos.y++;
		}
		else if (id == KEY_LEFT) {
			testPos.x--;
		}
		else if (id == KEY_RIGHT) {
			testPos.x++;
		}
		else if (id == KEY_A){
			rot(testTetromino, 1);
		}
		else if (id == KEY_B){
			rot(testTetromino, -1);
		}


		// check for collisions
		for(size_t i = 0; i < TETRA; i++){
			// check left wall
			if(testTetromino[i].x + testPos.x < 0){
				return;
			}

			// check right wall
			if(testTetromino[i].x + testPos.x >= DISPLAY_WIDTH){
				return;
			}

			// check floor
			if(testTetromino[i].y + testPos.y >= DISPLAY_HEIGHT){
				return;
			}

			// check heap
			int8_t x = testTetromino[i].x + testPos.x;
			int8_t y = testTetromino[i].y + testPos.y;

			if( x >= 0 && y >= 0 && x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT){
				if(heap[y][x] != BLACK){
					return;
				}
			}
		}

		// no collisions, accept movement
		currentPos = testPos;
		for(size_t i = 0; i < TETRA; i++){
			tetromino[i] = testTetromino[i];
		}
	}
}

void Tetris::rot(Point tetromino[TETRA], int dir){
	// dir = 1  : clockwise
	// dir = -1 : counterclockwise

	if(currentType == 0) {		// type O is not rotated
		return;
	}

	for(size_t i = 0; i < TETRA; i++){
		int8_t x = -dir * tetromino[i].y;
		int8_t y = dir * tetromino[i].x;
		tetromino[i].x = x;
		tetromino[i].y = y;
	}
}


void Tetris::placeNext(){

	// when pool is empty, fill it again
	if(tetrominoPool.size() == 0){
		for(size_t i = 0; i < N_TETROMINOS; i++)
		{
			tetrominoPool.push_back(i);
		}
	}

	// pick a random tetromino from pool
	size_t i = std::rand() % tetrominoPool.size();
	currentType = tetrominoPool[i];
	tetrominoPool.erase(tetrominoPool.begin() + i);

	currentColor = nextColor();
	currentPos.x = DISPLAY_WIDTH / 2;
	currentPos.y = 0;

	for(size_t i = 0; i < TETRA; i++)
	{
		tetromino[i] = Tetrominos[currentType][i];
	}
}

Tetris::Color Tetris::nextColor(){

	std::vector<Color> colorSet = ColorSet;

	// remove colors which are on top of the heap from the colorset
	for(size_t x = 0; x < DISPLAY_WIDTH; x++){
		for(size_t y = 0; y < DISPLAY_HEIGHT; y++){
			if(heap[y][x] != BLACK && heap[y][x] != WHITE){
				for(size_t i = 0; i < colorSet.size(); i++){
					if(colorSet[i] == heap[y][x]){
						colorSet.erase(colorSet.begin() + i);
						break;
					}
				}
				break;
			}
		}
	}

	// if there are no colors left, use all colors instead
	if(colorSet.size() == 0){
		colorSet = ColorSet;
	}

	// select random color from remaining colorSet
	return colorSet[std::rand() % colorSet.size()];
}


} // namespace pixelino::apps::tetris