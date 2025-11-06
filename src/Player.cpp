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
    
    // Update position and track distance
    float oldX = m_x;
    m_x += m_velX * deltaTime;
    m_y += m_velY * deltaTime;
    
    // Track distance traveled (only when moving right)
    if (m_x > oldX) {
        m_distanceTraveled += (m_x - oldX);
    }
    
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
    float leftLegOffset = 0;
    float rightLegOffset = 0;
    
    // Calculate animation frame for running (more exaggerated and smoother)
    if (m_animState == AnimationState::RUNNING_LEFT || m_animState == AnimationState::RUNNING_RIGHT) {
        // Smoother, more visible leg animation
        float animPhase = m_animationTime * 8.0f; // Slower animation speed
        leftLegOffset = std::sin(animPhase) * 12.0f; // Bigger leg movement
        rightLegOffset = std::sin(animPhase + 3.14159f) * 12.0f; // Opposite phase
        
        // Subtle body bounce
        bodyOffsetY = static_cast<int>(std::abs(std::sin(animPhase * 2.0f)) * 3);
    } else if (m_animState == AnimationState::JUMPING) {
        bodyOffsetY = -4;
        leftLegOffset = -8.0f; // Legs tucked up
        rightLegOffset = -8.0f;
    }
    
    // Draw cat body (oval/rounded)
    SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
    SDL_Rect bodyRect = {
        screenX + 8,
        screenY + 20 + bodyOffsetY,
        static_cast<int>(m_width - 16),
        static_cast<int>(m_height - 30)
    };
    SDL_RenderFillRect(renderer, &bodyRect);
    
    // Draw head (circular)
    SDL_Rect headRect = {
        screenX + 6,
        screenY + 5 + bodyOffsetY,
        28,
        22
    };
    SDL_RenderFillRect(renderer, &headRect);
    
    // Draw ears (triangular)
    SDL_SetRenderDrawColor(renderer, m_color.r - 20, m_color.g - 20, m_color.b, m_color.a);
    // Left ear
    for (int i = 0; i < 3; i++) {
        SDL_RenderDrawLine(renderer, 
            screenX + 10 + i, screenY + 5 + bodyOffsetY,
            screenX + 5 + i, screenY + bodyOffsetY);
        SDL_RenderDrawLine(renderer,
            screenX + 10 + i, screenY + 5 + bodyOffsetY,
            screenX + 15 + i, screenY + bodyOffsetY);
    }
    // Right ear
    for (int i = 0; i < 3; i++) {
        SDL_RenderDrawLine(renderer,
            screenX + 25 + i, screenY + 5 + bodyOffsetY,
            screenX + 20 + i, screenY + bodyOffsetY);
        SDL_RenderDrawLine(renderer,
            screenX + 25 + i, screenY + 5 + bodyOffsetY,
            screenX + 30 + i, screenY + bodyOffsetY);
    }
    
    // Draw eyes
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    int eyeY = screenY + 13 + bodyOffsetY;
    int leftEyeX = m_facingRight ? screenX + 13 : screenX + 11;
    int rightEyeX = m_facingRight ? screenX + 23 : screenX + 21;
    
    SDL_Rect leftEye = {leftEyeX, eyeY, 5, 5};
    SDL_Rect rightEye = {rightEyeX, eyeY, 5, 5};
    SDL_RenderFillRect(renderer, &leftEye);
    SDL_RenderFillRect(renderer, &rightEye);
    
    // Draw nose
    SDL_SetRenderDrawColor(renderer, 255, 182, 193, 255);
    SDL_Rect nose = {screenX + 18, screenY + 20 + bodyOffsetY, 4, 3};
    SDL_RenderFillRect(renderer, &nose);
    
    // Draw whiskers
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    int whiskerY = screenY + 18 + bodyOffsetY;
    SDL_RenderDrawLine(renderer, screenX + 5, whiskerY, screenX + 0, whiskerY - 2);
    SDL_RenderDrawLine(renderer, screenX + 5, whiskerY + 2, screenX + 0, whiskerY + 2);
    SDL_RenderDrawLine(renderer, screenX + 35, whiskerY, screenX + 40, whiskerY - 2);
    SDL_RenderDrawLine(renderer, screenX + 35, whiskerY + 2, screenX + 40, whiskerY + 2);
    
    // Draw TWO legs at the bottom (centered)
    SDL_SetRenderDrawColor(renderer, m_color.r - 40, m_color.g - 40, m_color.b - 10, m_color.a);
    int leftLegX = screenX + 14;
    int rightLegX = screenX + 22;
    
    // Each leg is 6 pixels wide
    for (int i = 0; i < 6; i++) {
        // Left leg
        SDL_RenderDrawLine(renderer,
            leftLegX + i, screenY + static_cast<int>(m_height) - 18 + bodyOffsetY,
            leftLegX + i + static_cast<int>(leftLegOffset / 3.0f), 
            screenY + static_cast<int>(m_height) + static_cast<int>(leftLegOffset) - 3);
        // Right leg
        SDL_RenderDrawLine(renderer,
            rightLegX + i, screenY + static_cast<int>(m_height) - 18 + bodyOffsetY,
            rightLegX + i + static_cast<int>(rightLegOffset / 3.0f), 
            screenY + static_cast<int>(m_height) + static_cast<int>(rightLegOffset) - 3);
    }
    
    // Draw paw pads at the end of legs
    SDL_SetRenderDrawColor(renderer, 255, 192, 203, 255); // Pink
    for (int dx = -3; dx <= 3; dx++) {
        for (int dy = -2; dy <= 2; dy++) {
            SDL_RenderDrawPoint(renderer, 
                leftLegX + 3 + static_cast<int>(leftLegOffset / 3.0f) + dx, 
                screenY + static_cast<int>(m_height) + static_cast<int>(leftLegOffset) - 3 + dy);
            SDL_RenderDrawPoint(renderer,
                rightLegX + 3 + static_cast<int>(rightLegOffset / 3.0f) + dx,
                screenY + static_cast<int>(m_height) + static_cast<int>(rightLegOffset) - 3 + dy);
        }
    }
    
    // Arms removed - they looked like extra feet
    
    // Draw tail (curved and wagging)
    SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
    int tailWag = m_animState == AnimationState::RUNNING_LEFT || m_animState == AnimationState::RUNNING_RIGHT 
                  ? static_cast<int>(std::sin(m_animationTime * 12.0f) * 10) : 0;
    int tailBaseX = m_facingRight ? screenX + static_cast<int>(m_width) - 8 : screenX + 8;
    int tailEndX = tailBaseX + (m_facingRight ? 12 : -12);
    
    for (int i = 0; i < 3; i++) {
        SDL_RenderDrawLine(renderer, 
            tailBaseX, screenY + 30 + bodyOffsetY + i,
            tailEndX, screenY + 15 + tailWag + bodyOffsetY + i);
    }
}

