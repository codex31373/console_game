#include "PhysicsWorld.hpp"
#include <iostream>

PhysicsWorld::PhysicsWorld()
    : m_world(nullptr)
    , m_timeAccumulator(0.0f)
{
    // Create Box2D world with gravity (9.8 m/s² downward)
    m_world = new b2World(b2Vec2(0.0f, 9.8f));
}

PhysicsWorld::~PhysicsWorld() {
    if (m_world) {
        delete m_world;
        m_world = nullptr;
    }
}

void PhysicsWorld::update(float deltaTime) {
    m_timeAccumulator += deltaTime;

    // Fixed time step for stable physics simulation
    while (m_timeAccumulator >= TIME_STEP) {
        m_world->Step(TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
        m_timeAccumulator -= TIME_STEP;
    }
}

b2Body* PhysicsWorld::createBody(const b2BodyDef& bodyDef) {
    return m_world->CreateBody(&bodyDef);
}

b2Fixture* PhysicsWorld::createFixture(b2Body* body, const b2FixtureDef& fixtureDef) {
    return body->CreateFixture(&fixtureDef);
}
