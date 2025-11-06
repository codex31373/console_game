#include "Game.hpp"
#include "GameObject.hpp"
#include "Player.hpp"
#include "Platform.hpp"
#include "Bird.hpp"
#include "Common.hpp"
#include <iostream>
#include <memory>
#include <algorithm>
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
    // Create ground with gaps
    float groundY = 550.0f;
    float currentX = 0.0f;
    float worldEnd = static_cast<float>(SCREEN_WIDTH * 3);
    
    // Create ground segments with gaps (VERY FREQUENT, but always crossable)
    for (int i = 0; i < 15; i++) {
        float segmentWidth = 120.0f + (rand() % 100); // Shorter segments = more gaps
        
        // Create ground segment
        auto groundSegment = std::make_unique<GameObject>(
            currentX, groundY,
            segmentWidth, 50.0f,
            Color{0, 128, 0, 255}
        );
        m_gameObjects.push_back(std::move(groundSegment));
        
        currentX += segmentWidth;
        
        // Add gaps VERY FREQUENTLY but keep them jumpable (max 120 pixels)
        // Player can jump about 140-150 pixels horizontally
        if (i > 0 && currentX < worldEnd - 200.0f && rand() % 2 == 0) { // 50% chance of gap (more frequent)
            float gapWidth = 60.0f + (rand() % 50); // Gap width 60-110 (always crossable)
            m_groundGaps.push_back({currentX, gapWidth});
            currentX += gapWidth;
        }
    }
    
    // Fill remaining ground
    if (currentX < worldEnd) {
        auto groundSegment = std::make_unique<GameObject>(
            currentX, groundY,
            worldEnd - currentX, 50.0f,
            Color{0, 128, 0, 255}
        );
        m_gameObjects.push_back(std::move(groundSegment));
    }

    // Create a series of platforms at different heights (FEWER platforms)
    const int numPlatforms = 10;  // Reduced from 15
    const float startX = 100.0f;
    const float platformSpacing = 180.0f;  // Increased spacing
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
    
    // Add VERY FEW obstacles - only essential ones for crossing
    for (int i = 0; i < 5; ++i) {  // Reduced from 8
        float x = 400.0f + i * 350.0f;  // Increased spacing
        
        // Add small obstacle RARELY (only 15% chance)
        if (i > 1 && rand() % 10 < 1) {  // Reduced from 3 to 1
            float height = 40.0f + (rand() % 20); // Smaller obstacles 40-60px
            auto vPlatform = std::make_unique<GameObject>(
                x, 550.0f - height,
                20.0f, height,
                Color{150, 75, 0, 255}
            );
            m_gameObjects.push_back(std::move(vPlatform));
        }
        
        // Add platforms at various heights for navigation (LESS FREQUENTLY)
        if (i % 3 == 1) {  // Changed from i % 2 == 1
            auto midPlatform = std::make_unique<Platform>(
                x - 60.0f, 480.0f,
                110.0f,
                Color{200, 150, 100, 255}
            );
            m_gameObjects.push_back(std::move(midPlatform));
        }
        
        if (i % 4 == 0) {  // Changed from i % 3 == 0
            auto topPlatform = std::make_unique<Platform>(
                x + 50.0f, 400.0f,
                100.0f,
                Color{100, 100, 255, 255}
            );
            m_gameObjects.push_back(std::move(topPlatform));
        }
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
    
    // Handle game over restart with Enter key
    if (m_gameOver && keyState[SDL_SCANCODE_RETURN]) {
        restartGame();
    }
    
    if (m_player && !m_gameOver) {
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
    
    // Check for game over
    if (m_gameOver) {
        // Wait for restart input
        return;
    }
    
    // Update invulnerability timer
    if (m_invulnerabilityTimer > 0.0f) {
        m_invulnerabilityTimer -= deltaTime;
    }

    // Check if we need to generate more platforms
    if (m_player->getX() > m_worldWidth - 1000.0f) {
        generateMorePlatformsIfNeeded();
    }
    
    // Spawn birds periodically
    m_birdSpawnTimer += deltaTime;
    if (m_birdSpawnTimer >= m_birdSpawnInterval) {
        spawnBirds();
        m_birdSpawnTimer = 0.0f;
    }
    
    // Update birds
    updateBirds(deltaTime);

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
    
    // Check if player fell into a gap BEFORE ground collision
    bool inGap = false;
    if (!onGround && playerBottom >= 545.0f) {
        float playerCenterX = m_player->getX() + m_player->getWidth() / 2.0f;
        for (const auto& gap : m_groundGaps) {
            if (playerCenterX >= gap.x && playerCenterX <= gap.x + gap.width) {
                inGap = true;
                break;
            }
        }
    }
    
    // Ground collision (only if not on a platform and not in a gap)
    if (!onGround && !inGap && playerBottom >= 545.0f) {
        m_player->setPosition(m_player->getX(), 550.0f - m_player->getHeight());
        m_player->setGrounded(true);
        m_player->setVelY(0.0f);
        onGround = true;
    }
    
    // If player is in gap and falling, check if they've fallen far enough
    if (inGap && playerBottom > 650.0f) {
        // Player has fallen into gap
        m_player->loseLife();
        std::cout << "Fell into a gap! Lives remaining: " << m_player->getLives() << std::endl;
        
        if (!m_player->isAlive()) {
            m_gameOver = true;
            std::cout << "GAME OVER!" << std::endl;
        } else {
            resetPlayer();
        }
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
    
    // Check bird collisions with player
    checkBirdCollisions();
}

void Game::generateMorePlatformsIfNeeded() {
    float currentEnd = m_worldWidth;
    m_worldWidth += 1000.0f;  // Extend the world by 1000 pixels
    
    // Add new ground segments with VERY FREQUENT gaps
    float currentX = currentEnd;
    for (int i = 0; i < 8; i++) {
        float segmentWidth = 120.0f + (rand() % 100);
        
        // Create ground segment
        auto groundSegment = std::make_unique<GameObject>(
            currentX, 550.0f,
            segmentWidth, 50.0f,
            Color{139, 69, 19, 255}
        );
        m_gameObjects.push_back(std::move(groundSegment));
        
        currentX += segmentWidth;
        
        // Add gap frequently (50% chance) but keep jumpable
        if (rand() % 2 == 0) {
            float gapWidth = 60.0f + (rand() % 50); // 60-110px (always crossable)
            m_groundGaps.push_back({currentX, gapWidth});
            currentX += gapWidth;
        }
    }
    
    // Generate new platforms with VERY FEW obstacles
    for (int i = 0; i < 6; ++i) {  // Reduced from 10
        float x = currentEnd + i * 200.0f;  // Increased spacing
        float y = 460.0f - 50.0f * sin((currentEnd + i * 200.0f) * 0.008f);
        
        // Main platform
        m_gameObjects.push_back(std::make_unique<Platform>(x, y, 110.0f));
        
        // Add obstacles VERY RARELY (only 10% chance)
        if (i % 6 == 0 && rand() % 10 < 1) {  // Reduced from 2 to 1
            float obstacleHeight = 40.0f + (rand() % 20); // 40-60px
            m_gameObjects.push_back(
                std::make_unique<GameObject>(x + 50.0f, 550.0f - obstacleHeight, 20.0f, obstacleHeight, Color{120, 80, 60, 255})
            );
        }
        
        // Add variety platforms occasionally (LESS FREQUENTLY)
        if (i % 4 == 1) {  // Changed from i % 3 == 1
            m_gameObjects.push_back(
                std::make_unique<Platform>(x + 70.0f, y - 70.0f, 90.0f, Color{150, 200, 150, 255})
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
    
    // Render birds
    for (const auto& bird : m_birds) {
        if (bird->isActive()) {
            bird->render(m_renderer);
        }
    }
    
    // Render player on top (flash when invulnerable)
    if (m_player) {
        bool showPlayer = true;
        if (m_invulnerabilityTimer > 0.0f) {
            // Flash effect
            showPlayer = (static_cast<int>(m_invulnerabilityTimer * 10) % 2 == 0);
        }
        if (showPlayer) {
            m_player->render(m_renderer);
        }
    }
    
    // Render UI (lives, game over, etc.)
    renderUI();

    // Update screen
    SDL_RenderPresent(m_renderer);
}

void Game::spawnBirds() {
    if (!m_player) return;
    
    // Spawn bird ahead of player at random height
    float spawnX = m_player->getX() + SCREEN_WIDTH + 100.0f;
    float spawnY = 100.0f + (rand() % 300); // Random height between 100-400
    
    // Random speed variation
    float speed = -120.0f - (rand() % 80); // Speed between -120 and -200
    
    auto bird = std::make_unique<Bird>(spawnX, spawnY, speed);
    bird->setCameraOffset(m_cameraX, 0);
    m_birds.push_back(std::move(bird));
}

void Game::updateBirds(float deltaTime) {
    // Update all birds
    for (auto& bird : m_birds) {
        if (bird->isActive()) {
            bird->update(deltaTime);
            bird->setCameraOffset(m_cameraX, 0);
        }
    }
    
    // Remove inactive birds
    m_birds.erase(
        std::remove_if(m_birds.begin(), m_birds.end(),
            [](const std::unique_ptr<Bird>& bird) { return !bird->isActive(); }),
        m_birds.end()
    );
}

void Game::checkBirdCollisions() {
    if (!m_player || m_invulnerabilityTimer > 0.0f) return;
    
    float playerLeft = m_player->getX();
    float playerRight = m_player->getX() + m_player->getWidth();
    float playerTop = m_player->getY();
    float playerBottom = m_player->getY() + m_player->getHeight();
    
    for (auto& bird : m_birds) {
        if (!bird->isActive()) continue;
        
        float birdLeft = bird->getLeft();
        float birdRight = bird->getRight();
        float birdTop = bird->getTop();
        float birdBottom = bird->getBottom();
        
        // AABB collision detection
        if (playerRight > birdLeft && playerLeft < birdRight &&
            playerBottom > birdTop && playerTop < birdBottom) {
            
            // Collision detected! Apply knockback and lose life
            bird->setActive(false);
            
            // Apply strong knockback - set velocity directly
            m_player->setVelY(-250.0f); // Push up
            m_player->setGrounded(false); // Make sure player can be knocked back
            
            // Push player back horizontally
            float pushBackDistance = 80.0f;
            m_player->setPosition(m_player->getX() - pushBackDistance, m_player->getY());
            
            // Lose a life
            m_player->loseLife();
            m_invulnerabilityTimer = 1.5f; // 1.5 seconds invulnerability
            
            std::cout << "Bird collision! Lives remaining: " << m_player->getLives() << std::endl;
            
            // Check for game over
            if (!m_player->isAlive()) {
                m_gameOver = true;
                std::cout << "GAME OVER!" << std::endl;
            }
            
            break; // Only process one collision per frame
        }
    }
}

void Game::resetPlayer() {
    if (!m_player) return;
    
    // Reset player to starting position
    m_player->setPosition(100.0f, 300.0f);
    m_player->setVelY(0.0f);
    m_invulnerabilityTimer = 2.0f; // Give invulnerability after respawn
}

void Game::restartGame() {
    if (!m_player) return;
    
    // Reset game state
    m_gameOver = false;
    m_player->resetLives();
    m_player->resetDistance();
    m_player->setPosition(100.0f, 300.0f);
    m_player->setVelY(0.0f);
    m_invulnerabilityTimer = 0.0f;
    
    // Clear birds
    m_birds.clear();
    m_birdSpawnTimer = 0.0f;
    
    // Reset world
    m_worldWidth = 2000.0f;
    m_cameraX = 0.0f;
    m_gameObjects.clear();
    m_groundGaps.clear();
    
    // Recreate level
    createLevel();
}

void Game::renderUI() {
    if (!m_renderer || !m_player) return;
    
    // Render lives (hearts) in top-left corner
    int lives = m_player->getLives();
    SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255); // Red hearts
    
    for (int i = 0; i < lives; i++) {
        int heartX = 20 + i * 40;
        int heartY = 20;
        int heartSize = 25;
        
        // Draw a simple heart shape (two circles and a triangle)
        SDL_Rect leftCircle = {heartX, heartY, heartSize/2, heartSize/2};
        SDL_Rect rightCircle = {heartX + heartSize/2, heartY, heartSize/2, heartSize/2};
        SDL_RenderFillRect(m_renderer, &leftCircle);
        SDL_RenderFillRect(m_renderer, &rightCircle);
        
        // Triangle bottom
        for (int y = 0; y < heartSize/2; y++) {
            SDL_RenderDrawLine(m_renderer, 
                heartX + y/2, heartY + heartSize/2 + y,
                heartX + heartSize - y/2, heartY + heartSize/2 + y);
        }
    }
    
    // Draw empty hearts for lost lives
    SDL_SetRenderDrawColor(m_renderer, 100, 100, 100, 255); // Gray
    for (int i = lives; i < 3; i++) {
        int heartX = 20 + i * 40;
        int heartY = 20;
        int heartSize = 25;
        
        SDL_Rect outline = {heartX, heartY, heartSize, heartSize};
        SDL_RenderDrawRect(m_renderer, &outline);
    }
    
    // Draw distance traveled in top-right corner
    float distance = m_player->getDistanceTraveled();
    int distanceMeters = static_cast<int>(distance / 10.0f); // Convert pixels to "meters"
    
    // Draw distance background
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 180);
    SDL_Rect distanceBg = {SCREEN_WIDTH - 200, 15, 180, 40};
    SDL_RenderFillRect(m_renderer, &distanceBg);
    
    // Draw distance label and number as text-like display
    SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255); // Green for distance
    
    // Simple number display - draw the distance as blocks
    char distStr[20];
    snprintf(distStr, sizeof(distStr), "DIST:%d", distanceMeters);
    
    // Draw each character as simple blocks
    int charX = SCREEN_WIDTH - 190;
    int charY = 25;
    for (int i = 0; distStr[i] != '\0' && i < 15; i++) {
        // Simple block representation of characters
        SDL_Rect charRect = {charX + i * 11, charY, 9, 15};
        SDL_RenderFillRect(m_renderer, &charRect);
    }
    
    // Render GAME OVER message if game is over
    if (m_gameOver) {
        // Draw semi-transparent overlay
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 200);
        SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(m_renderer, &overlay);
        
        // Draw GAME OVER box
        SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
        SDL_Rect gameOverBg = {SCREEN_WIDTH/2 - 220, SCREEN_HEIGHT/2 - 120, 440, 100};
        SDL_RenderFillRect(m_renderer, &gameOverBg);
        
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        SDL_Rect gameOverInner = {SCREEN_WIDTH/2 - 215, SCREEN_HEIGHT/2 - 115, 430, 90};
        SDL_RenderFillRect(m_renderer, &gameOverInner);
        
        // Draw final distance box
        SDL_SetRenderDrawColor(m_renderer, 100, 100, 255, 255);
        SDL_Rect distanceBg2 = {SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 - 40, 400, 50};
        SDL_RenderFillRect(m_renderer, &distanceBg2);
        
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        SDL_Rect distanceInner = {SCREEN_WIDTH/2 - 195, SCREEN_HEIGHT/2 - 35, 390, 40};
        SDL_RenderFillRect(m_renderer, &distanceInner);
        
        // Draw final distance text
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 255, 255);
        char finalDistStr[30];
        snprintf(finalDistStr, sizeof(finalDistStr), "FINAL DISTANCE: %d", distanceMeters);
        
        int textX = SCREEN_WIDTH/2 - 180;
        int textY = SCREEN_HEIGHT/2 - 25;
        for (int i = 0; finalDistStr[i] != '\0' && i < 25; i++) {
            SDL_Rect charRect = {textX + i * 9, textY, 8, 12};
            SDL_RenderFillRect(m_renderer, &charRect);
        }
        
        // Draw "INSERT COIN FOR NEW GAME" message
        SDL_SetRenderDrawColor(m_renderer, 255, 200, 0, 255); // Gold color
        SDL_Rect coinBg = {SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 + 40, 400, 50};
        SDL_RenderFillRect(m_renderer, &coinBg);
        
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_Rect coinInner = {SCREEN_WIDTH/2 - 195, SCREEN_HEIGHT/2 + 45, 390, 40};
        SDL_RenderFillRect(m_renderer, &coinInner);
        
        // Draw coin message text
        SDL_SetRenderDrawColor(m_renderer, 255, 200, 0, 255);
        const char* coinMsg = "INSERT COIN FOR A NEW GAME";
        int msgX = SCREEN_WIDTH/2 - 190;
        int msgY = SCREEN_HEIGHT/2 + 55;
        for (int i = 0; coinMsg[i] != '\0' && i < 25; i++) {
            SDL_Rect charRect = {msgX + i * 9, msgY, 8, 12};
            SDL_RenderFillRect(m_renderer, &charRect);
        }
    }
}
