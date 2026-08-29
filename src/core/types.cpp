#include "types.hpp"
#include <FastLED.h>

namespace pixelino::core {

Color Color::fromHSV(std::uint8_t hue, std::uint8_t sat, std::uint8_t val) {
    // FastLED's CHSV class handles the HSV -> RGB math instantly
    CRGB temp = CHSV(hue, sat, val);
    return Color{temp.r, temp.g, temp.b};
}

}