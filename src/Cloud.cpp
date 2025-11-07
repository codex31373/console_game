#include "Cloud.hpp"
#include <SDL2/SDL.h>
#include <initializer_list>

Cloud::Cloud(float x, float y, float width, float height)
    : GameObject(x, y, width, height, Color{255, 255, 255, 200}) { // white with some transparency
}

void Cloud::update(float deltaTime) {
    // Move cloud slowly to the left
    m_x -= m_speed * deltaTime;
    // Reset cloud position if it goes off screen
    if (m_x + m_width < 0) {
        m_x = 1024; // screen width
    }
}

void Cloud::render(SDL_Renderer* renderer) const {
    // Render a simple cloud as a filled white ellipse or circle approximation
    SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
    // Simple cloud: draw 3 overlapping circles
    int centerX = static_cast<int>(m_x);
    int centerY = static_cast<int>(m_y);
    int radius = static_cast<int>(m_height / 2);

    // Draw 3 circles for cloud shape
    for (int dx : std::initializer_list<int>{-radius, 0, radius}) {
        for (int dy : std::initializer_list<int>{-radius / 2, 0, radius / 2}) {
            int cx = centerX + dx;
            int cy = centerY + dy;
            for (int w = 0; w < radius * 2; ++w) {
                for (int h = 0; h < radius * 2; ++h) {
                    int distX = radius - w;
                    int distY = radius - h;
                    if ((distX * distX + distY * distY) <= (radius * radius)) {
                        SDL_RenderDrawPoint(renderer, cx + w - radius, cy + h - radius);
                    }
                }
            }
        }
    }
}
