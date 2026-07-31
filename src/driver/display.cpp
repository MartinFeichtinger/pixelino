#include "display.hpp"
#include "core/config.hpp"
#include "core/types.hpp"
#include "core/error_handler.hpp"
#include "core/error_types.hpp"
#include "core/system_logger.hpp"
#include <FastLED.h>
#include <Arduino.h>

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
	core::SystemLogger::getInstance().logSystemEvent(core::SystemEvent::DISPLAY_INIT);
}

void Display::show() {
	core::SystemLogger::getInstance().logDriverEvent("DISPLAY", "SHOWED OUT_BUFF");
	FastLED.show();
}

// clears the m_leds buffer
void Display::clear() {
	core::SystemLogger::getInstance().logDriverEvent("DISPLAY", "CLEARED OUT_BUFF");
	FastLED.clear();
}

void Display::fill(Color col) {
	core::SystemLogger::getInstance().logDriverEvent("DISPLAY", "FILLED <COL> OUT_BUFF");
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
		core::SystemLogger::getInstance().logDriverEvent("DISPLAY", "SET_PIXEL <POS><COL> OUT_BUFF");
		std::uint8_t ledIndex = getIndex(x, y);
		m_leds[ledIndex] = CRGB(col.r, col.g, col.b);
	}
	else {
		if (x >= core::config::display::width) {
			core::ErrorHandler::getInstance().handle(core::ErrorCode::PIXEL_OUT_OF_BOUND_X);
		}
		if (y >= core::config::display::height) {
			core::ErrorHandler::getInstance().handle(core::ErrorCode::PIXEL_OUT_OF_BOUND_Y);
		}
	}
}

std::uint8_t Display::getIndex(std::uint8_t x, std::uint8_t y) const {
	return (x + y*core::config::display::width);
}

}