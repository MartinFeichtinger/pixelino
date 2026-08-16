#pragma once
#include <cstdint>

namespace pixelino::core {

    struct Position {
        std::uint8_t x{0};
        std::uint8_t y{0};
    };

    class Color {
    public:
        std::uint8_t r{0};
        std::uint8_t g{0};
        std::uint8_t b{0};

        constexpr Color() = default;
        constexpr Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue)
            : r(red), g(green), b(blue) {}
        constexpr Color(std::uint32_t hex)
            : r((hex >> 16) & 0xFF), g((hex >> 8) & 0xFF), b(hex & 0xFF) {}

        // --- FastLED Color Utilities (Wrapped) ---
        
        // Creates a color from Hue (0-255), Saturation (0-255), Value/Brightness (0-255)
        static Color fromHSV(std::uint8_t hue, std::uint8_t sat, std::uint8_t val);

        // Fades this color toward black by a percentage/amount
        void fadeToBlackBy(std::uint8_t amount);

        // Smoothly blends two colors together (amount: 0 = 100% color A, 255 = 100% color B)
        static Color blend(const Color& a, const Color& b, std::uint8_t amount);

        // --- Common Color Shortcuts ---
        static constexpr Color Black()  { return Color{0x000000}; }
		static constexpr Color White()	{ return Color{0xFFFFFF}; }
        static constexpr Color Red()	{ return Color{0xFF0000}; }
        static constexpr Color Green()	{ return Color{0x00FF00}; }
        static constexpr Color Blue()	{ return Color{0x0000FF}; }
		static constexpr Color Yellow()	{ return Color{0xFFFF00}; }
		static constexpr Color Orange()	{ return Color{0xFF8800}; }
		static constexpr Color Purple()	{ return Color{0xFF00FF}; }
		static constexpr Color Cyan()	{ return Color{0x00FFFF}; }

    };

    struct Pixel {
        Position pos;
        Color col;
    };
}