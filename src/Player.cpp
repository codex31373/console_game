#include "Player.hpp"

Player::Player(float x, float y, float width, float height, const Color& color)
    : m_x(x), m_y(y)
    , m_width(width), m_height(height)
    , m_color(color) {}

void Player::handleInput(const Uint8* keyState) {
    m_velX = 0.0f;
    
    // Horizontal movement
    if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A]) {
        m_velX = -m_speed;
    }
    if (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D]) {
        m_velX = m_speed;
    }
    
    // Jumping (only when not climbing and on ground)
    if ((keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_SPACE]) && m_isGrounded && !m_isClimbing) {
        m_velY = m_jumpForce;
        m_isGrounded = false;
    }
}

void Player::update(float deltaTime) {
    // Apply gravity if not climbing
    const float gravity = 800.0f;
    if (!m_isClimbing) {
        m_velY += gravity * deltaTime;
    } else {
        // Climbing movement
        m_velY = 0;
        if (m_velX != 0) {  // If moving horizontally while climbing
            m_y -= m_climbSpeed * 0.5f * deltaTime;  // Move up slightly when climbing diagonally
        }
    }
    
    // Update position
    m_x += m_velX * deltaTime;
    m_y += m_velY * deltaTime;
    
    // Only prevent going off the left edge of the world
    if (m_x < 0) m_x = 0;
}

void Player::render(SDL_Renderer* renderer) const {
    // Draw player (simple rectangle for now)
    SDL_Rect playerRect = {
        static_cast<int>(m_x - m_cameraOffsetX),
        static_cast<int>(m_y - m_cameraOffsetY),
        static_cast<int>(m_width),
        static_cast<int>(m_height)
    };
    
    // Draw filled rectangle
    SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
    SDL_RenderFillRect(renderer, &playerRect);
    
    // Draw outline
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &playerRect);
    
    // Draw cat face (simple eyes and mouth)
    int eyeY = static_cast<int>(m_y - m_cameraOffsetY + m_height * 0.3f);
    int eyeSize = static_cast<int>(m_width * 0.15f);
    
    // Left eye
    SDL_Rect leftEye = {
        static_cast<int>(m_x - m_cameraOffsetX + m_width * 0.25f - eyeSize/2.0f),
        eyeY,
        eyeSize,
        eyeSize
    };
    
    // Right eye
    SDL_Rect rightEye = {
        static_cast<int>(m_x - m_cameraOffsetX + m_width * 0.75f - eyeSize/2.0f),
        eyeY,
        eyeSize,
        eyeSize
    };
    
    // Draw eyes (black)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &leftEye);
    SDL_RenderFillRect(renderer, &rightEye);
    
    // Draw mouth (simple line)
    SDL_RenderDrawLine(
        renderer,
        static_cast<int>(m_x - m_cameraOffsetX + m_width * 0.3f),
        static_cast<int>(m_y - m_cameraOffsetY + m_height * 0.7f),
        static_cast<int>(m_x - m_cameraOffsetX + m_width * 0.7f),
        static_cast<int>(m_y - m_cameraOffsetY + m_height * 0.7f)
    );
}

