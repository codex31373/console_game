#ifndef GAME_HPP
#define GAME_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <memory>
#include "GameObject.hpp"
#include "Player.hpp"
#include "Bird.hpp"
#include "Sign.hpp"
#include "Cloud.hpp"
#include "Water.hpp"
#include "Grass.hpp"

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
    void generateMorePlatformsIfNeeded();
    void spawnBirds();
    void updateBirds(float deltaTime);
    void checkBirdCollisions();
    void renderUI();
    void resetPlayer();
    void restartGame();

    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    TTF_Font* m_font = nullptr;
    bool m_isRunning = false;
    Uint32 m_lastFrameTime = 0;
    
    std::vector<std::unique_ptr<GameObject>> m_gameObjects;
    std::unique_ptr<Player> m_player;
    std::vector<std::unique_ptr<Bird>> m_birds;
    std::unique_ptr<Sign> m_sign;
    std::vector<std::unique_ptr<Cloud>> m_clouds;
    std::unique_ptr<Water> m_water;
    std::vector<std::unique_ptr<Grass>> m_grassPatches;

    // World and camera properties
    float m_worldWidth = 2000.0f;
    float m_cameraX = 0.0f;
    float m_cameraY = 0.0f;
    
    // Bird spawning
    float m_birdSpawnTimer = 0.0f;
    float m_birdSpawnInterval = 3.0f;
    
    // Ground gaps (position and width)
    struct Gap {
        float x;
        float width;
    };
    std::vector<Gap> m_groundGaps;
    
    // Game state
    bool m_gameOver = false;
    float m_invulnerabilityTimer = 0.0f;
};

#endif // GAME_HPP
