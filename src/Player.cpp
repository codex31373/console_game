#include "Player.hpp"
#include <cmath>

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
    
    // Update animation state
    m_animationTime += deltaTime;
    
    if (!m_isGrounded) {
        m_animState = AnimationState::JUMPING;
    } else if (m_velX > 0) {
        m_animState = AnimationState::RUNNING_RIGHT;
        m_facingRight = true;
    } else if (m_velX < 0) {
        m_animState = AnimationState::RUNNING_LEFT;
        m_facingRight = false;
    } else {
        m_animState = AnimationState::IDLE;
    }
}

void Player::render(SDL_Renderer* renderer) const {
    int screenX = static_cast<int>(m_x - m_cameraOffsetX);
    int screenY = static_cast<int>(m_y - m_cameraOffsetY);
    
    // Animation offsets
    int bodyOffsetY = 0;
    int legOffset = 0;
    
    // Calculate animation frame for running
    if (m_animState == AnimationState::RUNNING_LEFT || m_animState == AnimationState::RUNNING_RIGHT) {
        legOffset = static_cast<int>(std::sin(m_animationTime * 10.0f) * 3);
        bodyOffsetY = static_cast<int>(std::abs(std::sin(m_animationTime * 10.0f)) * 2);
    } else if (m_animState == AnimationState::JUMPING) {
        bodyOffsetY = -5; // Stretched when jumping
    }
    
    // Draw cat body
    SDL_Rect bodyRect = {
        screenX + 5,
        screenY + 15 + bodyOffsetY,
        static_cast<int>(m_width - 10),
        static_cast<int>(m_height - 25)
    };
    SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
    SDL_RenderFillRect(renderer, &bodyRect);
    
    // Draw head
    SDL_Rect headRect = {
        screenX + 8,
        screenY + 5 + bodyOffsetY,
        static_cast<int>(m_width - 16),
        20
    };
    SDL_RenderFillRect(renderer, &headRect);
    
    // Draw ears
    SDL_SetRenderDrawColor(renderer, m_color.r - 20, m_color.g - 20, m_color.b, m_color.a);
    int earSize = 8;
    SDL_Point leftEarPoints[] = {
        {screenX + 10, screenY + 5 + bodyOffsetY},
        {screenX + 10 - earSize/2, screenY + bodyOffsetY},
        {screenX + 10 + earSize, screenY + 5 + bodyOffsetY}
    };
    SDL_RenderDrawLines(renderer, leftEarPoints, 3);
    
    SDL_Point rightEarPoints[] = {
        {screenX + static_cast<int>(m_width) - 10 - earSize, screenY + 5 + bodyOffsetY},
        {screenX + static_cast<int>(m_width) - 10 + earSize/2, screenY + bodyOffsetY},
        {screenX + static_cast<int>(m_width) - 10, screenY + 5 + bodyOffsetY}
    };
    SDL_RenderDrawLines(renderer, rightEarPoints, 3);
    
    // Draw eyes
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    int eyeY = screenY + 12 + bodyOffsetY;
    int eyeSize = 4;
    
    int leftEyeX = m_facingRight ? screenX + 15 : screenX + 12;
    int rightEyeX = m_facingRight ? screenX + 25 : screenX + 22;
    
    SDL_Rect leftEye = {leftEyeX, eyeY, eyeSize, eyeSize};
    SDL_Rect rightEye = {rightEyeX, eyeY, eyeSize, eyeSize};
    SDL_RenderFillRect(renderer, &leftEye);
    SDL_RenderFillRect(renderer, &rightEye);
    
    // Draw nose
    SDL_SetRenderDrawColor(renderer, 255, 182, 193, 255); // Pink
    SDL_Rect nose = {screenX + static_cast<int>(m_width/2) - 2, screenY + 18 + bodyOffsetY, 4, 3};
    SDL_RenderFillRect(renderer, &nose);
    
    // Draw tail (wagging based on animation)
    SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
    int tailWag = m_animState == AnimationState::RUNNING_LEFT || m_animState == AnimationState::RUNNING_RIGHT 
                  ? static_cast<int>(std::sin(m_animationTime * 12.0f) * 8) : 0;
    int tailBaseX = m_facingRight ? screenX + static_cast<int>(m_width) - 5 : screenX + 5;
    SDL_RenderDrawLine(renderer, 
        tailBaseX, screenY + 25 + bodyOffsetY,
        tailBaseX + (m_facingRight ? 10 : -10), screenY + 15 + tailWag + bodyOffsetY);
    
    // Draw legs (animated when running)
    SDL_SetRenderDrawColor(renderer, m_color.r - 30, m_color.g - 30, m_color.b, m_color.a);
    // Front legs
    SDL_RenderDrawLine(renderer, screenX + 15, screenY + static_cast<int>(m_height) - 10 + bodyOffsetY,
                       screenX + 15, screenY + static_cast<int>(m_height) + legOffset);
    SDL_RenderDrawLine(renderer, screenX + 20, screenY + static_cast<int>(m_height) - 10 + bodyOffsetY,
                       screenX + 20, screenY + static_cast<int>(m_height) - legOffset);
    // Back legs  
    SDL_RenderDrawLine(renderer, screenX + 25, screenY + static_cast<int>(m_height) - 10 + bodyOffsetY,
                       screenX + 25, screenY + static_cast<int>(m_height) - legOffset);
    SDL_RenderDrawLine(renderer, screenX + 30, screenY + static_cast<int>(m_height) - 10 + bodyOffsetY,
                       screenX + 30, screenY + static_cast<int>(m_height) + legOffset);
}

