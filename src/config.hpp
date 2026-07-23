#pragma once

#include <cstdint>
#include <FastLED.h>

namespace config {

	namespace pin {
		constexpr std::uint8_t boot_button 	= 0;
		constexpr std::uint8_t onboard_led 	= 2;
		constexpr std::uint8_t display_data = 4;
	}

	namespace display {
		constexpr std::size_t num_leds = 64;
		constexpr EOrder color_order = GRB;
		
		using controller = WS2812B<pin::display_data, color_order>;
	}

}