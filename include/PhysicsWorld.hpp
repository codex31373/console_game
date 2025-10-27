#pragma once

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>

class PhysicsWorld {
public:
    PhysicsWorld();
    ~PhysicsWorld();

    void update(float deltaTime);
    b2World* getWorld() { return m_world; }

    // Helper methods for creating physics objects
    b2Body* createBody(const b2BodyDef& bodyDef);
    b2Fixture* createFixture(b2Body* body, const b2FixtureDef& fixtureDef);

private:
    b2World* m_world;
    float m_timeAccumulator;
    static constexpr float TIME_STEP = 1.0f / 60.0f;
    static constexpr int VELOCITY_ITERATIONS = 6;
    static constexpr int POSITION_ITERATIONS = 2;
};
