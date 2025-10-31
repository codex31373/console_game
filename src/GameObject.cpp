#include "GameObject.hpp"

GameObject::GameObject(float x, float y, float width, float height, const Color& color)
    : m_x(x), m_y(y)
    , m_width(width), m_height(height)
    , m_color(color) {}

void GameObject::render(SDL_Renderer* renderer) const {
    SDL_Rect rect = {
        static_cast<int>(m_x - m_cameraOffsetX),
        static_cast<int>(m_y - m_cameraOffsetY),
        static_cast<int>(m_width),
        static_cast<int>(m_height)
    };
    
    // Draw filled rectangle
    SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
    SDL_RenderFillRect(renderer, &rect);
    
    // Draw outline
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
}
