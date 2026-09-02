#pragma once

#include <cstdint>
#include <FastLED.h>
#include "core/config.hpp"
#include "core/types.hpp"

namespace pixelino::driver {

class Display {
public:
    // single global instance
    static Display& getInstance() { static Display instance; return instance; }
    Display(const Display&) = delete;
    void operator=(const Display&) = delete;    

	// basic functions
    void begin();
	void show();

    // bulk copy an array of core::Color into the display buffer
    void loadBuffer(const core::Color* buffer);

    // fill entiere display with one color
	void fill(core::Color col);

    // alows to manipulate single pixels (not recommented for many pixels!)
    void setPixel(const core::Pixel& pix);
    void setPixel(core::Position pos, core::Color col);
    void setPixel(uint8_t x, uint8_t y, core::Color col);

    // alows to get the color of an pixel (only for storing and serial output)
    // do not use for game purposes
    core::Color getPixel(uint8_t x, uint8_t y) const;

	// calibrate functions (not implemented yet)
	// hueSweep (rainbow)
	// ...

private:
    // private construtor
    Display() = default;

	// array containing the the color for each led, used by the FastLED library
    CRGB m_leds[core::config::display::num_leds];

	// carbrate coretion data (not implemented yet)
    
    // helper returns true if pixel is in display boundaries else throw error and return false
    bool checkBounds(std::uint8_t x, std::uint8_t y) const;
    
    // helper to map 2D coordinates to the 1D FastLED array, with return value (0 - num_leds-1)
    std::uint8_t getIndex(std::uint8_t x, std::uint8_t y) const; 
};

}