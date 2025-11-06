#include "Stair.hpp"

Stair::Stair(float x, float y, float width, float height, bool facingRight)
    : GameObject(x, y, width, height, {139, 69, 19, 255})  // Brown color
    , m_facingRight(facingRight) {}

void Stair::render(SDL_Renderer* renderer) const {
    // Draw the main stair rectangle
    SDL_Rect stairRect = {
        static_cast<int>(m_x - m_cameraOffsetX),
        static_cast<int>(m_y - m_cameraOffsetY),
        static_cast<int>(m_width),
        static_cast<int>(m_height)
    };
    
    // Draw filled rectangle
    SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
    SDL_RenderFillRect(renderer, &stairRect);
    
    // Draw steps
    const int stepCount = 5;
    float stepWidth = m_width / stepCount;
    float stepHeight = m_height / stepCount;
    
    SDL_SetRenderDrawColor(renderer, 160, 82, 45, 255);  // SaddleBrown
    for (int i = 0; i < stepCount; ++i) {
        SDL_Rect stepRect;
        if (m_facingRight) {
            stepRect = {
                static_cast<int>(m_x - m_cameraOffsetX + i * stepWidth),
                static_cast<int>(m_y - m_cameraOffsetY + i * stepHeight),
                static_cast<int>(stepWidth * (stepCount - i)),
                static_cast<int>(stepHeight)
            };
        } else {
            stepRect = {
                static_cast<int>(m_x - m_cameraOffsetX),
                static_cast<int>(m_y - m_cameraOffsetY + i * stepHeight),
                static_cast<int>(stepWidth * (i + 1)),
                static_cast<int>(stepHeight)
            };
        }
        SDL_RenderFillRect(renderer, &stepRect);
    }
    
    // Draw outline
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &stairRect);
}
