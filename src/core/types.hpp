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

        
        // creates a rgb color from Hue (0-255), Saturation (0-255), Value/Brightness (0-255)
        static Color fromHSV(std::uint8_t hue, std::uint8_t sat = 255, std::uint8_t val = 255);
    };

    namespace color {
        constexpr Color black  {0x000000};
        constexpr Color white  {0xFFFFFF};
        constexpr Color red    {0xFF0000};
        constexpr Color green  {0x00FF00};
        constexpr Color blue   {0x0000FF};
        constexpr Color yellow {0xFFFF00};
        constexpr Color orange {0xFF8800};
        constexpr Color purple {0xFF00FF};
        constexpr Color cyan   {0x00FFFF};
    }

    struct Pixel {
        union {
            Position pos;
            struct {
                std::uint8_t x;
                std::uint8_t y;
            };
        };

        Color col;

        constexpr Pixel() : pos{0, 0}, col{color::black} {}
        constexpr Pixel(std::uint8_t _x, std::uint8_t _y, Color _c) : pos{_x, _y}, col{_c} {}
        constexpr Pixel(Position _pos, Color _c) : pos{_pos}, col{_c} {}
    };
}