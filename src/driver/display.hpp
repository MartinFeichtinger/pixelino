#pragma once

#include <cstdint>
#include <FastLED.h>
#include "core/config.hpp"
#include "core/types.hpp"

namespace pixelino::driver {

class Display {
public:
    // single global instance
    static Display& getInstance();
    Display(const Display&) = delete;
    void operator=(const Display&) = delete;    

	// basic functions
    void begin();
	void show();
    void clear();

    // drawing funcitons
	void fill(core::Color col);
    void setPixel(const core::Pixel& pix);
    void setPixel(core::Position pos, core::Color col);
    void setPixel(uint8_t x, uint8_t y, core::Color col);

	// calibrate functions (not implemented yet)
	// hueSweep (rainbow)
	// ...

private:
    // private construtor
    Display() = default;

	// array containing the the color for each led, used by the FastLED library
    CRGB m_leds[core::config::display::num_leds];

	// carbrate coretion data (not implemented yet)
    
    // helper to map 2D coordinates to the 1D FastLED array, with return value (0 - num_leds-1)
    std::uint8_t getIndex(std::uint8_t x, std::uint8_t y) const; 
};

}