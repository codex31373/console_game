#include "Bird.hpp"

Bird::Bird(float x, float y, float speed)
    : GameObject(x, y, 40.0f, 30.0f, Color{80, 80, 80, 255})
    , m_speed(speed)
    , m_animationTime(0.0f)
    , m_active(true)
    , m_wingUp(false) {
}

void Bird::update(float deltaTime) {
    // Move bird horizontally (flying left/opposite to player direction)
    m_x += m_speed * deltaTime;
    
    // Animate wings
    m_animationTime += deltaTime;
    if (m_animationTime > 0.2f) {
        m_wingUp = !m_wingUp;
        m_animationTime = 0.0f;
    }
    
    // Deactivate if bird goes off screen (left side)
    if (m_x < -100.0f) {
        m_active = false;
    }
}

void Bird::render(SDL_Renderer* renderer) const {
    if (!m_active) return;
    
    int screenX = static_cast<int>(m_x - m_cameraOffsetX);
    int screenY = static_cast<int>(m_y - m_cameraOffsetY);
    
    // Draw bird body (oval shape using rectangles)
    SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
    
    // Body
    SDL_Rect body = {
        screenX + 10, screenY + 10,
        static_cast<int>(m_width - 20), static_cast<int>(m_height - 10)
    };
    SDL_RenderFillRect(renderer, &body);
    
    // Head
    SDL_Rect head = {
        screenX + 5, screenY + 5,
        15, 15
    };
    SDL_RenderFillRect(renderer, &head);
    
    // Beak (yellow/orange)
    SDL_SetRenderDrawColor(renderer, 255, 180, 0, 255);
    SDL_Rect beak = { screenX + 2, screenY + 10, 8, 5 };
    SDL_RenderFillRect(renderer, &beak);
    
    // Eye (white with black pupil)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect eye = { screenX + 10, screenY + 8, 4, 4 };
    SDL_RenderFillRect(renderer, &eye);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect pupil = { screenX + 11, screenY + 9, 2, 2 };
    SDL_RenderFillRect(renderer, &pupil);
    
    // Wings (animated)
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    if (m_wingUp) {
        // Wings up
        SDL_RenderDrawLine(renderer, screenX + 15, screenY + 15, screenX + 5, screenY + 5);
        SDL_RenderDrawLine(renderer, screenX + 15, screenY + 15, screenX + 25, screenY + 5);
        SDL_RenderDrawLine(renderer, screenX + 5, screenY + 5, screenX + 0, screenY + 8);
        SDL_RenderDrawLine(renderer, screenX + 25, screenY + 5, screenX + 30, screenY + 8);
    } else {
        // Wings down
        SDL_RenderDrawLine(renderer, screenX + 15, screenY + 15, screenX + 5, screenY + 25);
        SDL_RenderDrawLine(renderer, screenX + 15, screenY + 15, screenX + 25, screenY + 25);
        SDL_RenderDrawLine(renderer, screenX + 5, screenY + 25, screenX + 0, screenY + 22);
        SDL_RenderDrawLine(renderer, screenX + 25, screenY + 25, screenX + 30, screenY + 22);
    }
}
