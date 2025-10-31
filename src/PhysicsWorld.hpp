#ifndef PHYSICS_WORLD_HPP
#define PHYSICS_WORLD_HPP

#include <Box2D/Box2D.h>

class PhysicsWorld {
public:
    PhysicsWorld();
    ~PhysicsWorld();

    b2World* getWorld() const { return m_world; }
    void step(float timeStep);

private:
    b2World* m_world;
};

#endif // PHYSICS_WORLD_HPP
