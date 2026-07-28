#pragma once

#include <cstdint>
#include <FastLED.h>

namespace pixelino::core::config {

	namespace gpio {
		constexpr std::uint8_t onboard_button	= 0;
		constexpr std::uint8_t onboard_led		= 2;
		constexpr std::uint8_t display_data		= 4;
		constexpr std::uint8_t key_up			= 22;
		constexpr std::uint8_t key_down			= 21;
		constexpr std::uint8_t key_left			= 23;
		constexpr std::uint8_t key_right		= 18;
		constexpr std::uint8_t key_a			= 15;
		constexpr std::uint8_t key_b			= 19;
		constexpr std::uint8_t key_select		= 5;
		constexpr std::uint8_t key_start		= 16;
	}

	namespace display {
		constexpr std::uint8_t width = 8;
		constexpr std::uint8_t height = 8;
		constexpr int num_leds = width*height; 
		constexpr EOrder color_order = GRB;
		constexpr std::uint8_t brightness = 20;

		// FastLED's strict templates reject modern C++ aliases, so we wrap the raw initialization here.
		// This avoids messy #define macros and keeps the main Display constructor hardware-agnostic.
		inline void init(CRGB* leds) {
			FastLED.addLeds<WS2812B, gpio::display_data, color_order>(leds, num_leds);
		}
	}

	namespace error {
		constexpr std::uint8_t max_log_entries = 50;
	}

}