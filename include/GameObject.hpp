#pragma once

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>
#include <SDL2/SDL.h>

struct Color {
    Uint8 r, g, b, a;
};

class GameObject {
public:
    GameObject(b2World* world, float x, float y, float width, float height, const Color& color = {255, 255, 255, 255});
    virtual ~GameObject();

    virtual void update(float deltaTime);
    virtual void render(SDL_Renderer* renderer);

    // Physics body access
    b2Body* getBody() { return m_body; }
    const b2Body* getBody() const { return m_body; }

    // Transform methods
    void setPosition(float x, float y);
    void setRotation(float angle);
    b2Vec2 getPosition() const;

protected:
    b2Body* m_body;
    SDL_Rect m_renderRect;
    Color m_color;

    // Convert between Box2D and SDL coordinates
    b2Vec2 worldToScreen(const b2Vec2& worldPos) const;
    b2Vec2 screenToWorld(const b2Vec2& screenPos) const;

private:
    static constexpr float PIXELS_PER_METER = 50.0f;
};
