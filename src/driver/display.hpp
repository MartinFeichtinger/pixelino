#pragma once

#include <cstdint>
#include <FastLED.h>
#include "config.hpp"
#include "types.hpp"

namespace Engine {

class Display {
public:
	// construtor (init function)
    Display();

	// basic functions
	void show();
    void clear();

    // drawing funcitons
	void fill(Color col);
    void setPixel(const Pixel& pix);
    void setPixel(Position pos, Color col);
    void setPixel(uint8_t x, uint8_t y, Color col);

	// calibrate functions (not implemented yet)
	// hueSweep (rainbow)
	// ...

private:
	// array containing the the color for each led, used by the FastLED library
    CRGB m_leds[config::display::num_leds];

	// carbrate coretion data (not implemented yet)
    
    // helper to map 2D coordinates to the 1D FastLED array, with return value (0 - num_leds-1)
    std::uint8_t getIndex(std::uint8_t x, std::uint8_t y) const; 
};

}