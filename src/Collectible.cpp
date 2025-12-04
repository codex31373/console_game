#include "Collectible.hpp"
#include "Player.hpp"
#include <cstdlib>
#include <cmath>
#include <iostream>

Collectible::Collectible(float x, float y, float speed)
    : GameObject(x, y, 20.0f, 20.0f, Color{255, 255, 0, 255})  // Default to yellow coin
    , m_speed(speed) {
    // Randomly determine the type of collectible
    int type = rand() % 3;
    switch (type) {
        case 0: 
            m_type = Type::COIN;
            m_color = {255, 215, 0, 255};  // Gold
            break;
        case 1:
            m_type = Type::GEM;
            m_color = {0, 255, 255, 255};  // Cyan
            break;
        case 2:
            m_type = Type::STAR;
            m_color = {255, 255, 0, 255};  // Yellow
            break;
    }
}

void Collectible::update(float deltaTime) {
    if (!m_active) return;
    
    // Move left
    m_x += m_speed * deltaTime;
    
    // Add some vertical oscillation
    m_oscillation += deltaTime * 3.0f;
    m_y += std::sin(m_oscillation) * 0.5f;
    
    // Deactivate if off screen to the left
    if (m_x + m_width < m_cameraOffsetX) {
        m_active = false;
    }
}

bool Collectible::checkCollision(const GameObject& other) const {
    if (!m_active) return false;
    
    // Simple AABB (Axis-Aligned Bounding Box) collision detection
    bool collisionX = getRight() > other.getX() && getX() < (other.getX() + other.getWidth());
    bool collisionY = getBottom() > other.getY() && getY() < (other.getY() + other.getHeight());
    
    return collisionX && collisionY;
}

void Collectible::onCollected(Player& player) {
    if (!m_active) return;
    
    // Suppress unused parameter warning for now
    (void)player;
    
    // Apply different effects based on collectible type
    switch (m_type) {
        case Type::COIN: {
            // Add points for collecting a coin
            // player.addScore(100);  // TODO: Implement score system
            std::cout << "Collected a coin! +100 points" << std::endl;
            break;
        }
        case Type::GEM: {
            // Add more points for a gem and give temporary invincibility
            // player.addScore(500);  // TODO: Implement score system
            // player.setInvincible(5.0f); // TODO: Implement invincibility system
            std::cout << "Collected a gem! +500 points and temporary invincibility" << std::endl;
            break;
        }
        case Type::STAR: {
            // Add points and restore health
            // player.addScore(200);  // TODO: Implement score system
            // player.heal(1); // TODO: Implement health system
            std::cout << "Collected a star! +200 points and health restored" << std::endl;
            break;
        }
    }
    
    // Deactivate the collectible
    m_active = false;
    
    // Play collection sound (if sound system is implemented)
    // playSound("collect.wav");
}

void Collectible::render(SDL_Renderer* renderer) const {
    if (!m_active) return;
    
    SDL_Rect rect = {
        static_cast<int>(m_x - m_cameraOffsetX),
        static_cast<int>(m_y - m_cameraOffsetY),
        static_cast<int>(m_width),
        static_cast<int>(m_height)
    };
    
    // Draw different shapes based on type
    switch (m_type) {
        case Type::COIN: {
            // Draw a circle for coin
            SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
            for (int w = 0; w < rect.w; ++w) {
                for (int h = 0; h < rect.h; ++h) {
                    int dx = w - rect.w/2;
                    int dy = h - rect.h/2;
                    if (dx*dx + dy*dy <= (rect.w/2)*(rect.w/2)) {
                        SDL_RenderDrawPoint(renderer, rect.x + w, rect.y + h);
                    }
                }
            }
            break;
        }
        case Type::GEM: {
            // Draw a diamond shape for gem
            SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
            for (int w = 0; w < rect.w; ++w) {
                for (int h = 0; h < rect.h; ++h) {
                    if (std::abs(w - rect.w/2) + std::abs(h - rect.h/2) <= rect.w/2) {
                        SDL_RenderDrawPoint(renderer, rect.x + w, rect.y + h);
                    }
                }
            }
            break;
        }
        case Type::STAR: {
            // Draw a simple star
            SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
            for (int w = 0; w < rect.w; ++w) {
                for (int h = 0; h < rect.h; ++h) {
                    int dx = w - rect.w/2;
                    int dy = h - rect.h/2;
                    int dist = dx*dx + dy*dy;
                    if (dist > (rect.w/4)*(rect.w/4) && dist <= (rect.w/2)*(rect.w/2)) {
                        if (std::abs(dx) < rect.w/4 || std::abs(dy) < rect.h/4) {
                            SDL_RenderDrawPoint(renderer, rect.x + w, rect.y + h);
                        }
                    }
                }
            }
            break;
        }
    }
    
    // Draw outline
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
}
