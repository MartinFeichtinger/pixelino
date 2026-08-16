#include "display.hpp"
#include "core/config.hpp"
#include "core/types.hpp"
#include "core/error_handler.hpp"
#include "core/error_types.hpp"
#include "core/system_logger.hpp"
#include <FastLED.h>
#include <Arduino.h>
#include <cstring>

namespace pixelino::driver {

using core::Color;
using core::Position;
using core::Pixel;

void Display::begin() {
    core::config::display::init(m_leds);
	FastLED.setDither(BINARY_DITHER);
  	FastLED.setBrightness(core::config::display::brightness);
	core::SystemLogger::getInstance().logEvent(core::LogSource::DRIVER, "DISPLAY", "DISPLAY INITIATED");
}

void Display::show() {
	//core::SystemLogger::getInstance().logEvent(core::LogSource::DRIVER, "DISPLAY", "SHOWED OUT_BUFF");
	FastLED.show();
}

// clears the m_leds buffer
void Display::clear() {
	//core::SystemLogger::getInstance().logEvent(core::LogSource::DRIVER, "DISPLAY", "CLEARED OUT_BUFF");
	FastLED.clear();
}

void Display::loadBuffer(const core::Color* buffer, std::size_t count) {
    static_assert(sizeof(core::Color) == sizeof(CRGB), "Color memory layout mismatch");
    std::size_t copyBytes = std::min(count, static_cast<std::size_t>(core::config::display::num_leds)) * sizeof(CRGB);
    
    std::memcpy(m_leds, buffer, copyBytes);

	//core::SystemLogger::getInstance().logEvent(core::LogSource::DRIVER, "DISPLAY", "EXTERNAL BUFFER LOADED");
}

void Display::fill(Color col) {
	//core::SystemLogger::getInstance().logEvent(core::LogSource::DRIVER, "DISPLAY", "FILLED <COL> OUT_BUFF");
	fill_solid(m_leds, core::config::display::num_leds, CRGB(col.r, col.g, col.b));
}

void Display::setPixel(const Pixel& pix) {
	setPixel(pix.pos.x, pix.pos.y, pix.col);
}

void Display::setPixel(Position pos, Color col) {
	setPixel(pos.x, pos.y, col);
}

void Display::setPixel(uint8_t x, uint8_t y, Color col) {
	if (checkBounds(x, y)) {
		//core::SystemLogger::getInstance().logEvent(core::LogSource::DRIVER, "DISPLAY", "SET_PIXEL <POS><COL> OUT_BUFF");
		std::uint8_t ledIndex = getIndex(x, y);
		m_leds[ledIndex] = CRGB(col.r, col.g, col.b);
	}
}

core::Color Display::getPixel(uint8_t x, uint8_t y) const{
	if (checkBounds(x, y)) {
		std::uint8_t index = getIndex(x, y);
		CRGB col = m_leds[getIndex(x, y)];
		return core::Color(col.r, col.g, col.b);
	}
}

bool Display::checkBounds(std::uint8_t x, std::uint8_t y) const {
	if (x < core::config::display::width && y < core::config::display::height)
	{
		return true;
	}
	else {
		if (x >= core::config::display::width) {
			core::ErrorHandler::getInstance().handle(core::ErrorCode::PIXEL_OUT_OF_BOUND_X);
		}
		if (y >= core::config::display::height) {
			core::ErrorHandler::getInstance().handle(core::ErrorCode::PIXEL_OUT_OF_BOUND_Y);
		}
		return false;
	}
}

std::uint8_t Display::getIndex(std::uint8_t x, std::uint8_t y) const {
	return (x + y*core::config::display::width);
}

} // namespace pixelino::driver