#include "GameObject.hpp"
#include <cmath>

GameObject::GameObject(b2World* world, float x, float y, float width, float height, const Color& color)
    : m_color(color)
{
    // Create Box2D body definition
    b2BodyDef bodyDef;
    bodyDef.position.Set(x / PIXELS_PER_METER, y / PIXELS_PER_METER);
    bodyDef.type = b2_staticBody; // Default to static

    // Create the body
    m_body = world->CreateBody(&bodyDef);

    // Create Box2D shape
    b2PolygonShape boxShape;
    boxShape.SetAsBox((width / 2.0f) / PIXELS_PER_METER, (height / 2.0f) / PIXELS_PER_METER);

    // Create fixture definition
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.5f; // Bounciness

    // Create the fixture
    m_body->CreateFixture(&fixtureDef);

    // Store render rectangle for SDL rendering
    m_renderRect.w = static_cast<int>(width);
    m_renderRect.h = static_cast<int>(height);
}

GameObject::~GameObject() {
    // Body cleanup is handled by the physics world
}

void GameObject::update([[maybe_unused]] float deltaTime) {
    // Update render position based on physics body position
    b2Vec2 pos = m_body->GetPosition();
    m_renderRect.x = static_cast<int>((pos.x * PIXELS_PER_METER) - (m_renderRect.w / 2));
    m_renderRect.y = static_cast<int>((pos.y * PIXELS_PER_METER) - (m_renderRect.h / 2));
}

void GameObject::render(SDL_Renderer* renderer) {
    // Set render color
    SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);

    // Render filled rectangle
    SDL_RenderFillRect(renderer, &m_renderRect);

    // Render outline for better visibility
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &m_renderRect);
}

void GameObject::setPosition(float x, float y) {
    m_body->SetTransform(b2Vec2(x / PIXELS_PER_METER, y / PIXELS_PER_METER), m_body->GetAngle());
}

void GameObject::setRotation(float angle) {
    m_body->SetTransform(m_body->GetPosition(), angle);
}

b2Vec2 GameObject::getPosition() const {
    return m_body->GetPosition();
}

b2Vec2 GameObject::worldToScreen(const b2Vec2& worldPos) const {
    return b2Vec2(worldPos.x * PIXELS_PER_METER, worldPos.y * PIXELS_PER_METER);
}

b2Vec2 GameObject::screenToWorld(const b2Vec2& screenPos) const {
    return b2Vec2(screenPos.x / PIXELS_PER_METER, screenPos.y / PIXELS_PER_METER);
}
