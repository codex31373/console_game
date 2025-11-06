#ifndef BIRD_HPP
#define BIRD_HPP

#include "GameObject.hpp"
#include <cmath>

class Bird : public GameObject {
public:
    Bird(float x, float y, float speed = -150.0f);
    
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) const override;
    
    bool isActive() const { return m_active; }
    void setActive(bool active) { m_active = active; }
    
    // Get bird bounds for collision detection
    float getLeft() const { return m_x; }
    float getRight() const { return m_x + m_width; }
    float getTop() const { return m_y; }
    float getBottom() const { return m_y + m_height; }
    
private:
    float m_speed;
    float m_animationTime;
    bool m_active;
    bool m_wingUp;
};

#endif // BIRD_HPP
