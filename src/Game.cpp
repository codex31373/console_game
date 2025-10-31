#include "Game.hpp"
#include "GameObject.hpp"
#include "Player.hpp"
#include "Common.hpp"
#include <iostream>
#include <memory>
#include <cstdlib>  // For rand()

Game::Game() = default;

Game::~Game() {
    shutdown();
}

bool Game::initialize() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create window
    m_window = SDL_CreateWindow(
        "Cat Game",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!m_window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create renderer with VSYNC
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Set initial game state
    m_isRunning = true;
    m_lastFrameTime = SDL_GetTicks();

    // Create level objects
    createLevel();

    // Create the player (orange cat)
    m_player = std::make_unique<Player>(100.0f, 300.0f, 40.0f, 60.0f, Color{255, 165, 0, 255});

    return true;
}

void Game::createLevel() {
    // Create ground
    auto ground = std::make_unique<GameObject>(
        0.0f, 550.0f,
        static_cast<float>(SCREEN_WIDTH), 50.0f,
        Color{0, 128, 0, 255}  // Green ground
    );
    m_gameObjects.push_back(std::move(ground));

    // Create some platforms
    auto platform1 = std::make_unique<GameObject>(
        200.0f, 400.0f,
        200.0f, 20.0f,
        Color{100, 100, 255, 255}  // Blue platform
    );
    m_gameObjects.push_back(std::move(platform1));

    auto platform2 = std::make_unique<GameObject>(
        500.0f, 300.0f,
        200.0f, 20.0f,
        Color{100, 100, 255, 255}  // Blue platform
    );
    m_gameObjects.push_back(std::move(platform2));
}

void Game::run() {
    while (m_isRunning) {
        processEvents();

        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - m_lastFrameTime) / 1000.0f;
        m_lastFrameTime = currentTime;

        // Cap delta time to prevent physics issues
        if (deltaTime > 0.1f) {
            deltaTime = 0.1f;
        }

        update(deltaTime);
        render();
    }
}

void Game::shutdown() {
    m_gameObjects.clear();
    m_player.reset();
    
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    
    SDL_Quit();
}

void Game::processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            m_isRunning = false;
        }
        handleInput(event);
    }
    
    // Handle continuous key states for smooth movement
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);
    if (m_player) {
        m_player->handleInput(keyState);
    }
}

bool Game::handleInput(const SDL_Event& event) {
    switch (event.type) {
        case SDL_QUIT:
            return false;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                return false;
            }
            break;
    }
    return true;
}

void Game::update(float deltaTime) {
    // Update player
    if (m_player) {
        m_player->update(deltaTime);
        
        // Simple camera follow - center on player
        m_cameraX = m_player->getX() - SCREEN_WIDTH / 2.0f;
        m_cameraX = std::max(0.0f, std::min(m_cameraX, 800.0f - SCREEN_WIDTH));
        
        // Apply camera offset to all objects
        for (auto& obj : m_gameObjects) {
            obj->setCameraOffset(m_cameraX, 0);
        }
        m_player->setCameraOffset(m_cameraX, 0);
        
        // Simple collision with ground (y=550 is ground level)
        if (m_player->getY() + m_player->getHeight() > 550.0f) {
            m_player->setPosition(m_player->getX(), 550.0f - m_player->getHeight());
            m_player->setGrounded(true);
        }
    }
    
    // Update other game objects
    for (auto& obj : m_gameObjects) {
        obj->update(deltaTime);
    }
}

void Game::render() {
    // Clear screen with light blue background
    SDL_SetRenderDrawColor(m_renderer, 135, 206, 250, 255); // Light sky blue
    SDL_RenderClear(m_renderer);

    // Render all game objects
    for (const auto& obj : m_gameObjects) {
        obj->render(m_renderer);
    }
    
    // Render player on top
    if (m_player) {
        m_player->render(m_renderer);
    }

    // Update screen
    SDL_RenderPresent(m_renderer);
}
