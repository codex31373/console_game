#ifndef GAME_HPP
#define GAME_HPP

#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include "GameObject.hpp"
#include "Player.hpp"

class Game {
public:
    static const int SCREEN_WIDTH = 1024;
    static const int SCREEN_HEIGHT = 768;

    Game();
    ~Game();

    bool initialize();
    void run();
    void shutdown();

private:
    void processEvents();
    bool handleInput(const SDL_Event& event);
    void update(float deltaTime);
    void render();
    
    void createLevel();

    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    bool m_isRunning = false;
    Uint32 m_lastFrameTime = 0;
    
    std::vector<std::unique_ptr<GameObject>> m_gameObjects;
    std::unique_ptr<Player> m_player;
    
    // Camera offset for scrolling
    float m_cameraX = 0.0f;
    float m_cameraY = 0.0f;
};

#endif // GAME_HPP
