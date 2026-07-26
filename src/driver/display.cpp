#include "display.hpp"
#include <Arduino.h>
#include <FastLED.h>
#include "core/config.hpp"
#include "core/types.hpp"
#include "core/error_handler.hpp"
#include "core/error_types.hpp"

namespace pixelino::driver {

using core::Color;
using core::Position;
using core::Pixel;


Display& Display::getInstance() {
    static Display instance;
    return instance;
}

void Display::begin() {
    core::config::display::init(m_leds);
	FastLED.setDither(BINARY_DITHER);
  	FastLED.setBrightness(core::config::display::brightness);
}

void Display::show() {
	FastLED.show();
}

// clears the m_leds buffer
void Display::clear() {
	FastLED.clear();
}

void Display::fill(Color col) {
	fill_solid(m_leds, core::config::display::num_leds, CRGB(col.r, col.g, col.b));
}

void Display::setPixel(const Pixel& pix) {
	setPixel(pix.pos.x, pix.pos.y, pix.col);
}

void Display::setPixel(Position pos, Color col) {
	setPixel(pos.x, pos.y, col);
}

void Display::setPixel(uint8_t x, uint8_t y, Color col) {
	if (x < core::config::display::width && y < core::config::display::height) {
		std::uint8_t ledIndex = getIndex(x, y);
		m_leds[ledIndex] = CRGB(col.r, col.g, col.b);
	}
	else {
		core::ErrorHandler::getInstance().handle(core::ErrorCode::PIXEL_OUT_OF_BOUNDS);
	}
}

std::uint8_t Display::getIndex(std::uint8_t x, std::uint8_t y) const {
	return (x + y*core::config::display::width);
}

}