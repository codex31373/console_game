#ifndef CLOUD_HPP
#define CLOUD_HPP

#include "GameObject.hpp"

class Cloud : public GameObject {
public:
    Cloud(float x, float y, float width, float height);
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) const override;
    void setCameraOffset(float, float) override { /* Ignore camera movement */ }

private:
    float m_speed = 20.0f; // pixels per second
};

#endif // CLOUD_HPP
