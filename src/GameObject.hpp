#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <SDL2/SDL.h>
#include "Common.hpp"

class GameObject {
public:
    GameObject(float x, float y, float width, float height, const Color& color);
    virtual ~GameObject() = default;

    virtual void update([[maybe_unused]] float deltaTime) {}
    virtual void render(SDL_Renderer* renderer) const;
    
    void setPosition(float x, float y) { m_x = x; m_y = y; }
    float getX() const { return m_x; }
    float getY() const { return m_y; }
    float getWidth() const { return m_width; }
    float getHeight() const { return m_height; }
    
    virtual void setCameraOffset(float x, float y) { m_cameraOffsetX = x; m_cameraOffsetY = y; }

protected:
    float m_x, m_y;
    float m_width, m_height;
    Color m_color;
    float m_cameraOffsetX = 0.0f;
    float m_cameraOffsetY = 0.0f;
};

#endif // GAME_OBJECT_HPP
