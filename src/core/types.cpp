#include "types.hpp"
#include <FastLED.h>

namespace pixelino::core {

Color Color::fromHSV(std::uint8_t hue, std::uint8_t sat, std::uint8_t val) {
    // FastLED's CHSV class handles the HSV -> RGB math instantly
    CRGB temp = CHSV(hue, sat, val);
    return Color{temp.r, temp.g, temp.b};
}

void Color::fadeToBlackBy(std::uint8_t amount) {
    CRGB temp(r, g, b);
    temp.fadeToBlackBy(amount); // FastLED's optimized fading
    r = temp.r;
    g = temp.g;
    b = temp.b;
}

Color Color::blend(const Color& a, const Color& b, std::uint8_t amount) {
    CRGB colorA(a.r, a.g, a.b);
    CRGB colorB(b.r, b.g, b.b);
    
    // FastLED's nblend does smooth color interpolation
    CRGB blended = nblend(colorA, colorB, amount);
    return Color{blended.r, blended.g, blended.b};
}

}