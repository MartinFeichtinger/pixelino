#pragma once

#include <cstdint>
#include <FastLED.h>

namespace pixelino::core::config {

    namespace pin {
        constexpr std::uint8_t boot_button  = 0;
        constexpr std::uint8_t onboard_led  = 2;
        constexpr std::uint8_t display_data = 4;
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
            FastLED.addLeds<WS2812B, pin::display_data, color_order>(leds, num_leds);
        }
    }

}