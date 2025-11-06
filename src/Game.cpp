#include "Game.hpp"
#include "GameObject.hpp"
#include "Player.hpp"
#include "Platform.hpp"
#include "Common.hpp"
#include <iostream>
#include <memory>
#include <cstdlib>  // For rand()

Game::Game() : m_window(nullptr), m_renderer(nullptr), m_isRunning(false), m_worldWidth(2000.0f), m_cameraX(0.0f), m_cameraY(0.0f) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
}

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
        static_cast<float>(SCREEN_WIDTH * 3),  // Extend ground
        50.0f,
        Color{0, 128, 0, 255}  // Green ground
    );
    m_gameObjects.push_back(std::move(ground));

    // Create a series of platforms at different heights
    const int numPlatforms = 15;
    const float startX = 100.0f;
    const float platformSpacing = 150.0f;
    const float platformWidth = 100.0f;
    
    // Create platforms in a wave-like pattern
    for (int i = 0; i < numPlatforms; ++i) {
        float x = startX + i * platformSpacing;
        // Create a wave pattern for the y-coordinate
        float y = 450.0f - 50.0f * std::sin(i * 0.7f);
        
        // Random platform color
        Color color = {
            static_cast<Uint8>(100 + rand() % 156),  // R: 100-255
            static_cast<Uint8>(100 + rand() % 156),  // G: 100-255
            static_cast<Uint8>(100 + rand() % 156),  // B: 100-255
            255
        };
        
        auto platform = std::make_unique<Platform>(x, y, platformWidth, color);
        m_gameObjects.push_back(std::move(platform));
    }
    
    // Add some vertical platforms for climbing
    for (int i = 0; i < 5; ++i) {
        float x = 400.0f + i * 200.0f;
        float height = 100.0f + (i % 3) * 50.0f;
        
        // Vertical platform
        auto vPlatform = std::make_unique<GameObject>(
            x, 550.0f - height,
            30.0f, height,
            Color{150, 75, 0, 255}  // Brown for vertical platforms
        );
        m_gameObjects.push_back(std::move(vPlatform));
        
        // Small platform on top
        auto topPlatform = std::make_unique<Platform>(
            x - 50.0f, 550.0f - height - 20.0f,
            130.0f,
            Color{100, 100, 255, 255}  // Blue for top platforms
        );
        m_gameObjects.push_back(std::move(topPlatform));
    }
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
    if (!m_player) return;

    // Check if we need to generate more platforms
    if (m_player->getX() > m_worldWidth - 1000.0f) {
        generateMorePlatformsIfNeeded();
    }

    // Update player first
    m_player->update(deltaTime);
    
    // Reset grounded state - will be set to true if standing on something
    m_player->setGrounded(false);
    
    // Check for platform collisions
    bool onGround = false;
    float playerBottom = m_player->getY() + m_player->getHeight();
    float playerLeft = m_player->getX();
    float playerRight = m_player->getX() + m_player->getWidth();
    
    for (const auto& obj : m_gameObjects) {
        // Check collision with platforms
        if (auto platform = dynamic_cast<Platform*>(obj.get())) {
            float platformTop = platform->getY();
            float platformLeft = platform->getX();
            float platformRight = platform->getX() + platform->getWidth();
            
            // Check if player is standing on or landing on platform
            // More lenient check: player's bottom is within range of platform top
            if (playerBottom >= platformTop - 5.0f && 
                playerBottom <= platformTop + 15.0f &&
                playerRight > platformLeft + 2.0f && 
                playerLeft < platformRight - 2.0f) {
                
                // Snap player to top of platform
                m_player->setPosition(m_player->getX(), platformTop - m_player->getHeight());
                m_player->setGrounded(true);
                // Reset vertical velocity to prevent falling through
                if (m_player->getVelY() > 0.0f) {
                    m_player->setVelY(0.0f);
                }
                onGround = true;
                break;
            }
        }
        // Check collision with vertical platforms (walls) - but skip the ground
        else if (auto wall = dynamic_cast<GameObject*>(obj.get())) {
            // Only treat as wall if height > width AND it's not the ground (ground is very wide)
            if (wall->getHeight() > wall->getWidth() && wall->getWidth() < 100.0f) {
                float wallLeft = wall->getX();
                float wallRight = wall->getX() + wall->getWidth();
                float wallTop = wall->getY();
                float wallBottom = wall->getY() + wall->getHeight();
                
                // Only check wall collision if player is within the vertical bounds
                if (playerBottom > wallTop + 5.0f && m_player->getY() < wallBottom - 5.0f) {
                    // Check for left side collision (player moving right into wall)
                    if (playerRight > wallLeft && playerRight < wallLeft + 15.0f) {
                        m_player->setPosition(wallLeft - m_player->getWidth(), m_player->getY());
                    }
                    // Check for right side collision (player moving left into wall)
                    else if (playerLeft < wallRight && playerLeft > wallRight - 15.0f) {
                        m_player->setPosition(wallRight, m_player->getY());
                    }
                }
            }
        }
    }
    
    // Ground collision (only if not on a platform)
    if (!onGround && playerBottom >= 545.0f) {
        m_player->setPosition(m_player->getX(), 550.0f - m_player->getHeight());
        m_player->setGrounded(true);
        m_player->setVelY(0.0f);
        onGround = true;
    }
    
    // Update camera to follow player
    m_cameraX = m_player->getX() - SCREEN_WIDTH / 2.0f;
    m_cameraX = std::max(0.0f, std::min(m_cameraX, m_worldWidth - SCREEN_WIDTH));    
    // Apply camera offset to all objects and player
    for (auto& obj : m_gameObjects) {
        obj->setCameraOffset(m_cameraX, 0);
    }
    m_player->setCameraOffset(m_cameraX, 0);
    
    // Update other game objects
    for (auto& obj : m_gameObjects) {
        obj->update(deltaTime);
    }
}

void Game::generateMorePlatformsIfNeeded() {
    float currentEnd = m_worldWidth;
    m_worldWidth += 1000.0f;  // Extend the world by 1000 pixels
    
    // Extend the ground
    m_gameObjects[0] = std::make_unique<GameObject>(0.0f, 550.0f, m_worldWidth, 50.0f, Color{139, 69, 19, 255});
    
    // Generate new platforms in the extended area
    for (int i = 0; i < 10; ++i) {
        float x = currentEnd + i * 150.0f;
        float y = 400.0f + 100.0f * sin((currentEnd + i * 150.0f) * 0.01f);
        m_gameObjects.push_back(std::make_unique<Platform>(x, y, 100.0f));
        
        // Add some vertical walls with platforms occasionally
        if (i % 3 == 0) {
            float wallHeight = 200.0f + (rand() % 100);
            m_gameObjects.push_back(
                std::make_unique<GameObject>(x + 50.0f, 550.0f - wallHeight, 30.0f, wallHeight, Color{100, 100, 100, 255})
            );
            m_gameObjects.push_back(
                std::make_unique<Platform>(x + 80.0f, 550.0f - wallHeight - 30.0f, 100.0f)
            );
        }
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
