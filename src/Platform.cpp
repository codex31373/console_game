#include "Platform.hpp"
#include <cmath>
#include <random>

Platform::Platform(float x, float y, float width, const Color& color)
    : GameObject(x, y, width, HEIGHT, color) {
    generateTexturePattern();
}

void Platform::generateTexturePattern() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    
    m_texturePattern.resize(TEXTURE_PATTERN_SIZE);
    for (int i = 0; i < TEXTURE_PATTERN_SIZE; ++i) {
        float t = static_cast<float>(i) / TEXTURE_PATTERN_SIZE;
        float noise = 0.5f + 0.5f * std::sin(t * 6.0f) * std::cos(t * 9.0f) * std::sin(t * 15.0f);
        m_texturePattern[i] = 0.3f + 0.7f * noise * dist(gen);
    }
}

void Platform::update(float deltaTime) {
    // Static platforms - no animation needed
    (void)deltaTime; // Suppress unused parameter warning
}

void Platform::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    SDL_Rect platformRect = {
        static_cast<int>(m_x - m_cameraOffsetX),
        static_cast<int>(m_y - m_cameraOffsetY),
        static_cast<int>(m_width),
        static_cast<int>(m_height)
    };
    
    // Create beautiful wave-like gradient background
    for (int y = 0; y < platformRect.h; ++y) {
        float gradientT = static_cast<float>(y) / platformRect.h;
        
        // Wave-inspired color transitions like water and clouds
        float wave1 = std::sin(gradientT * 2.0f * 3.14159f);
        float wave2 = std::cos(gradientT * 3.0f * 3.14159f);
        
        SDL_Color gradientColor = {
            static_cast<Uint8>(139 + 60 * wave1 + 20 * wave2),
            static_cast<Uint8>(69 + 40 * wave1 + 15 * wave2),
            static_cast<Uint8>(19 + 25 * wave1 + 10 * wave2),
            255
        };
        
        SDL_SetRenderDrawColor(renderer, gradientColor.r, gradientColor.g, gradientColor.b, gradientColor.a);
        SDL_Rect gradientRect = {
            platformRect.x,
            platformRect.y + y,
            platformRect.w,
            1
        };
        SDL_RenderFillRect(renderer, &gradientRect);
    }
    
    // Draw artistic texture pattern with cloud-like softness
    int textureSegments = 25;
    float segmentWidth = m_width / textureSegments;
    
    for (int i = 0; i < textureSegments; ++i) {
        float gradientT = static_cast<float>(i) / textureSegments;
        
        // Cloud-like soft color variations
        float cloudWave = std::sin(gradientT * 4.0f * 3.14159f) * 0.5f + 0.5f;
        
        SDL_Color textureColor = {
            static_cast<Uint8>(120 + 40 * cloudWave),
            static_cast<Uint8>(60 + 30 * cloudWave),
            static_cast<Uint8>(20 + 15 * cloudWave),
            140
        };
        
        SDL_Rect textureRect = {
            static_cast<int>(m_x - m_cameraOffsetX + i * segmentWidth),
            static_cast<int>(m_y - m_cameraOffsetY),
            static_cast<int>(segmentWidth) + 1,  // +1 to prevent gaps
            static_cast<int>(m_height)
        };
        
        SDL_SetRenderDrawColor(renderer, textureColor.r, textureColor.g, textureColor.b, textureColor.a);
        SDL_RenderFillRect(renderer, &textureRect);
    }
    
    // Add cloud-like soft highlight on top edge
    for (int highlight = 0; highlight < 4; ++highlight) {
        float alpha = 180 - highlight * 35;
        SDL_Color highlightColor = {
            static_cast<Uint8>(210 + highlight * 15),
            static_cast<Uint8>(130 + highlight * 10),
            static_cast<Uint8>(70 + highlight * 8),
            static_cast<Uint8>(alpha)
        };
        
        SDL_SetRenderDrawColor(renderer, highlightColor.r, highlightColor.g, highlightColor.b, highlightColor.a);
        SDL_Rect highlightRect = {
            platformRect.x,
            platformRect.y + highlight,
            platformRect.w,
            1
        };
        SDL_RenderFillRect(renderer, &highlightRect);
    }
    
    // Add soft shadow on bottom edge
    for (int shadow = 0; shadow < 3; ++shadow) {
        float alpha = 100 - shadow * 25;
        SDL_SetRenderDrawColor(renderer, 40, 25, 10, static_cast<Uint8>(alpha));
        SDL_Rect shadowRect = {
            platformRect.x,
            platformRect.y + platformRect.h - 1 - shadow,
            platformRect.w,
            1
        };
        SDL_RenderFillRect(renderer, &shadowRect);
    }
    
    // Draw artistic outline with wave-like gradient
    for (int i = 0; i < 3; ++i) {
        float gradientT = static_cast<float>(i) / 3.0f;
        float wave = std::sin(gradientT * 3.14159f);
        
        SDL_Color outlineColor = {
            static_cast<Uint8>(100 + 50 * wave),
            static_cast<Uint8>(50 + 25 * wave),
            static_cast<Uint8>(15 + 10 * wave),
            255
        };
        
        SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
        SDL_Rect outlineRect = {
            platformRect.x - i,
            platformRect.y - i,
            platformRect.w + i * 2,
            platformRect.h + i * 2
        };
        SDL_RenderDrawRect(renderer, &outlineRect);
    }
}

bool Platform::canStandOn(float playerBottom) const {
    // Check if player's bottom is near the top of the platform
    return playerBottom >= m_y - 5.0f && playerBottom <= m_y + 5.0f;
}
