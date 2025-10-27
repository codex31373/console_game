#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <vector>

#include "PhysicsWorld.hpp"
#include "GameObject.hpp"

class Game {
public:
    Game();
    ~Game();

    bool initialize();
    void run();
    void shutdown();

private:
    void processEvents();
    void update(float deltaTime);
    void render();

    bool handleInput(const SDL_Event& event);

    // SDL components
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

    // Game state
    bool m_isRunning;
    Uint32 m_lastFrameTime;

    // Physics and game objects
    std::unique_ptr<PhysicsWorld> m_physicsWorld;
    std::vector<std::unique_ptr<GameObject>> m_gameObjects;

    // Constants
    static constexpr int SCREEN_WIDTH = 800;
    static constexpr int SCREEN_HEIGHT = 600;
    static constexpr float TIME_STEP = 1.0f / 60.0f;
};
