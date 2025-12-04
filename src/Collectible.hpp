#ifndef COLLECTIBLE_HPP
#define COLLECTIBLE_HPP

#include "GameObject.hpp"
#include "Common.hpp"
#include "Player.hpp"

class Player; // Forward declaration

class Collectible : public GameObject {
public:
    Collectible(float x, float y, float speed);
    ~Collectible() override = default;

    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) const override;
    
    bool isActive() const { return m_active; }
    void setActive(bool active) { m_active = active; }
    
    // Get bounding box for collision detection
    float getLeft() const { return m_x; }
    float getRight() const { return m_x + m_width; }
    float getTop() const { return m_y; }
    float getBottom() const { return m_y + m_height; }
    
    // Check collision with another game object
    bool checkCollision(const GameObject& other) const;
    
    // Handle collection by player
    void onCollected(Player& player);
    
    // Unique identifier for the collectible type
    enum class Type { COIN, GEM, STAR };
    Type getType() const { return m_type; }

private:
    bool m_active = true;
    float m_speed;
    float m_oscillation = 0.0f;
    Type m_type;
};

#endif // COLLECTIBLE_HPP
