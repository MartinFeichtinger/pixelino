#pragma once
#include <cstdint>

namespace engine {

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

        // --- FastLED Color Utilities (Wrapped) ---
        
        // Creates a color from Hue (0-255), Saturation (0-255), Value/Brightness (0-255)
        static Color fromHSV(std::uint8_t hue, std::uint8_t sat, std::uint8_t val);

        // Fades this color toward black by a percentage/amount
        void fadeToBlackBy(std::uint8_t amount);

        // Smoothly blends two colors together (amount: 0 = 100% color A, 255 = 100% color B)
        static Color blend(const Color& a, const Color& b, std::uint8_t amount);

        // --- Common Color Shortcuts ---
        static constexpr Color Red()   { return Color{255, 0, 0}; }
        static constexpr Color Green() { return Color{0, 255, 0}; }
        static constexpr Color Blue()  { return Color{0, 0, 255}; }
    };

    struct Pixel {
        Position pos;
        Color col;
    };

}