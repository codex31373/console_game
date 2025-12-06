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
        screenX + 8, screenY + 10,
        static_cast<int>(m_width - 16), static_cast<int>(m_height - 14)
    };
    SDL_RenderFillRect(renderer, &body);
    
    // Head
    SDL_Rect head = {
        screenX + 4, screenY + 6,
        14, 14
    };
    SDL_RenderFillRect(renderer, &head);
    
    // Beak (yellow/orange)
    SDL_SetRenderDrawColor(renderer, 255, 200, 80, 255);
    SDL_Rect beak = { screenX + 0, screenY + 11, 8, 4 };
    SDL_RenderFillRect(renderer, &beak);
    
    // Eye (white with black pupil)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect eye = { screenX + 10, screenY + 8, 4, 4 };
    SDL_RenderFillRect(renderer, &eye);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect pupil = { screenX + 11, screenY + 9, 2, 2 };
    SDL_RenderFillRect(renderer, &pupil);
    
    // Wings (animated)
    SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
    SDL_Rect wing;
    if (m_wingUp) {
        // Wings up
        wing = { screenX + 18, screenY + 6, 16, 10 };
    } else {
        // Wings down
        wing = { screenX + 18, screenY + 14, 16, 10 };
    }
    SDL_RenderFillRect(renderer, &wing);
}
