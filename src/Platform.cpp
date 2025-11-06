#include "Platform.hpp"
#include <SDL2/SDL.h>

Platform::Platform(float x, float y, float width, const Color& color)
    : GameObject(x, y, width, HEIGHT, color) {}

void Platform::render(SDL_Renderer* renderer) const {
    SDL_Rect platformRect = {
        static_cast<int>(m_x - m_cameraOffsetX),
        static_cast<int>(m_y - m_cameraOffsetY),
        static_cast<int>(m_width),
        static_cast<int>(m_height)
    };
    
    // Draw platform
    SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
    SDL_RenderFillRect(renderer, &platformRect);
    
    // Draw border
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &platformRect);
}

bool Platform::canStandOn(float playerBottom) const {
    // Check if player's bottom is near the top of the platform
    return playerBottom >= m_y - 5.0f && playerBottom <= m_y + 5.0f;
}
