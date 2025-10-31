#ifndef COMMON_HPP
#define COMMON_HPP

#include <SDL2/SDL.h>

struct Color {
    Uint8 r, g, b, a;
    
    Color() : r(0), g(0), b(0), a(255) {}
    Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {}
};

#endif // COMMON_HPP
