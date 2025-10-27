#include "Game.hpp"
#include "GameObject.hpp"
#include <iostream>
#include <memory>

Game::Game()
    : m_window(nullptr)
    , m_renderer(nullptr)
    , m_isRunning(false)
    , m_lastFrameTime(0)
    , m_physicsWorld(nullptr)
{
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
        "Console Game",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (m_window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create renderer
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (m_renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize physics world
    m_physicsWorld = std::make_unique<PhysicsWorld>();

    // Set initial game state
    m_isRunning = true;
    m_lastFrameTime = SDL_GetTicks();

    // Create some initial game objects
    // Ground
    auto ground = std::make_unique<GameObject>(
        m_physicsWorld->getWorld(),
        SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 20.0f,
        SCREEN_WIDTH, 40.0f,
        Color{0, 255, 0, 255} // Green ground
    );
    ground->getBody()->SetType(b2_staticBody);
    m_gameObjects.push_back(std::move(ground));

    // Some dynamic objects
    for (int i = 0; i < 5; ++i) {
        auto box = std::make_unique<GameObject>(
            m_physicsWorld->getWorld(),
            100.0f + i * 120.0f, 100.0f,
            40.0f, 40.0f,
            Color{255, static_cast<Uint8>(100 + i * 30), 100, 255} // Different colored boxes
        );
        box->getBody()->SetType(b2_dynamicBody);
        m_gameObjects.push_back(std::move(box));
    }

    return true;
}

void Game::run() {
    while (m_isRunning) {
        processEvents();

        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - m_lastFrameTime) / 1000.0f;
        m_lastFrameTime = currentTime;

        // Cap delta time to prevent spiral of death
        if (deltaTime > 0.1f) {
            deltaTime = 0.1f;
        }

        update(deltaTime);
        render();
    }
}

void Game::shutdown() {
    m_gameObjects.clear();
    m_physicsWorld.reset();

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
        if (!handleInput(event)) {
            m_isRunning = false;
        }
    }
}

bool Game::handleInput(const SDL_Event& event) {
    switch (event.type) {
        case SDL_QUIT:
            return false;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    return false;
                case SDLK_SPACE:
                    // Add a new dynamic object when space is pressed
                    auto newBox = std::make_unique<GameObject>(
                        m_physicsWorld->getWorld(),
                        static_cast<float>(rand() % SCREEN_WIDTH), 50.0f,
                        30.0f, 30.0f,
                        Color{static_cast<Uint8>(rand() % 255), static_cast<Uint8>(rand() % 255), static_cast<Uint8>(rand() % 255), 255}
                    );
                    newBox->getBody()->SetType(b2_dynamicBody);
                    m_gameObjects.push_back(std::move(newBox));
                    break;
            }
            break;
    }
    return true;
}

void Game::update(float deltaTime) {
    // Update physics
    m_physicsWorld->update(deltaTime);

    // Update game objects
    for (auto& obj : m_gameObjects) {
        obj->update(deltaTime);
    }
}

void Game::render() {
    // Clear screen
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    // Render game objects
    for (auto& obj : m_gameObjects) {
        obj->render(m_renderer);
    }

    // Present rendered frame
    SDL_RenderPresent(m_renderer);
}
