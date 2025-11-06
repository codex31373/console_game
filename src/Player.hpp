#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL2/SDL.h>
#include "Common.hpp"

class Player {
public:
    Player(float x, float y, float width, float height, const Color& color);
    
    // Input and game loop methods
    void handleInput(const Uint8* keyState);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer) const;
    
    // Position and size getters/setters
    void setPosition(float x, float y) { m_x = x; m_y = y; }
    float getX() const { return m_x; }
    float getY() const { return m_y; }
    float getWidth() const { return m_width; }
    float getHeight() const { return m_height; }
    
    // Camera control
    void setCameraOffset(float x, float y) { 
        m_cameraOffsetX = x; 
        m_cameraOffsetY = y; 
    }
    
    // Movement states
    void setGrounded(bool grounded) { m_isGrounded = grounded; }
    bool isClimbing() const { return m_isClimbing; }
    void setClimbing(bool climbing) { m_isClimbing = climbing; }
    float getVelY() const { return m_velY; }
    void setVelY(float velY) { m_velY = velY; }
    
private:
    // Position and size
    float m_x, m_y;
    float m_width, m_height;
    
    // Physics
    float m_velX = 0, m_velY = 0;
    float m_speed = 200.0f;
    float m_climbSpeed = 150.0f;
    float m_jumpForce = -400.0f;
    bool m_isGrounded = false;
    bool m_isClimbing = false;
    
    // Rendering
    Color m_color;
    float m_cameraOffsetX = 0.0f;
    float m_cameraOffsetY = 0.0f;
};

#endif // PLAYER_HPP
