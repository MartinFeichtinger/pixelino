#include "display.hpp"
#include <Arduino.h>
#include <FastLED.h>
#include "config.hpp"
#include "types.hpp"

engine::Display& engine::Display::getInstance() {
    static Display instance;
    return instance;
}

void engine::Display::begin() {
    config::display::init(m_leds);
	FastLED.setDither(BINARY_DITHER);
  	FastLED.setBrightness(config::display::brightness);
}

void engine::Display::show() {
	FastLED.show();
}

// clears the m_leds buffer
void engine::Display::clear() {
	FastLED.clear();
}

void engine::Display::fill(Color col) {
	fill_solid(m_leds, config::display::num_leds, CRGB(col.r, col.g, col.b));
}

void engine::Display::setPixel(const Pixel& pix) {
	setPixel(pix.pos.x, pix.pos.y, pix.col);
}

void engine::Display::setPixel(Position pos, Color col) {
	setPixel(pos.x, pos.y, col);
}

void engine::Display::setPixel(uint8_t x, uint8_t y, Color col) {
	if (x < config::display::width && y < config::display::height) {
		std::uint8_t ledIndex = getIndex(x, y);
		m_leds[ledIndex] = CRGB(col.r, col.g, col.b);
	}
	else {
		// further error handling in the futur
	}
}

std::uint8_t engine::Display::getIndex(std::uint8_t x, std::uint8_t y) const {
	return (x + y*config::display::width);
}