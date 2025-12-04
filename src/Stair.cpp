#include "Stair.hpp"
#include <cmath>
#include <random>

Stair::Stair(float x, float y, float width, float height, bool facingRight)
    : GameObject(x, y, width, height, {139, 69, 19, 255})  // Brown color
    , m_facingRight(facingRight) {
    generateShimmerPattern();
}

void Stair::generateShimmerPattern() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    
    m_shimmerPattern.resize(SHIMMER_PATTERN_SIZE);
    for (int i = 0; i < SHIMMER_PATTERN_SIZE; ++i) {
        float t = static_cast<float>(i) / SHIMMER_PATTERN_SIZE;
        float noise = 0.5f + 0.5f * std::sin(t * 8.0f) * std::cos(t * 12.0f);
        m_shimmerPattern[i] = 0.4f + 0.6f * noise * dist(gen);
    }
}

void Stair::update(float deltaTime) {
    // Static stairs - no animation needed
    (void)deltaTime; // Suppress unused parameter warning
}

void Stair::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    // Draw the main stair rectangle with beautiful gradient
    SDL_Rect stairRect = {
        static_cast<int>(m_x - m_cameraOffsetX),
        static_cast<int>(m_y - m_cameraOffsetY),
        static_cast<int>(m_width),
        static_cast<int>(m_height)
    };
    
    // Create beautiful gradient background like waves
    for (int y = 0; y < stairRect.h; ++y) {
        float gradientT = static_cast<float>(y) / stairRect.h;
        
        // Wave-like color transitions
        SDL_Color gradientColor = {
            static_cast<Uint8>(139 + 40 * std::sin(gradientT * 3.14159f)),
            static_cast<Uint8>(69 + 30 * std::cos(gradientT * 3.14159f)),
            static_cast<Uint8>(19 + 15 * std::sin(gradientT * 2.0f * 3.14159f)),
            255
        };
        
        SDL_SetRenderDrawColor(renderer, gradientColor.r, gradientColor.g, gradientColor.b, gradientColor.a);
        SDL_Rect gradientRect = {
            stairRect.x,
            stairRect.y + y,
            stairRect.w,
            1
        };
        SDL_RenderFillRect(renderer, &gradientRect);
    }
    
    // Draw steps with artistic wave-like gradients
    const int stepCount = 5;
    float stepWidth = m_width / stepCount;
    float stepHeight = m_height / stepCount;
    
    for (int i = 0; i < stepCount; ++i) {
        SDL_Rect stepRect;
        if (m_facingRight) {
            stepRect = {
                static_cast<int>(m_x - m_cameraOffsetX + i * stepWidth),
                static_cast<int>(m_y - m_cameraOffsetY + i * stepHeight),
                static_cast<int>(stepWidth * (stepCount - i)),
                static_cast<int>(stepHeight)
            };
        } else {
            stepRect = {
                static_cast<int>(m_x - m_cameraOffsetX),
                static_cast<int>(m_y - m_cameraOffsetY + i * stepHeight),
                static_cast<int>(stepWidth * (i + 1)),
                static_cast<int>(stepHeight)
            };
        }
        
        // Create wave-like gradient for each step
        for (int y = 0; y < stepRect.h; ++y) {
            float localGradientT = static_cast<float>(y) / stepRect.h;
            float globalGradientT = static_cast<float>(i) / stepCount;
            
            // Beautiful wave-inspired color transitions
            float wave1 = std::sin(localGradientT * 2.0f * 3.14159f + globalGradientT * 3.14159f);
            float wave2 = std::cos(localGradientT * 3.0f * 3.14159f - globalGradientT * 2.0f * 3.14159f);
            
            SDL_Color stepColor = {
                static_cast<Uint8>(160 + 60 * wave1 + 20 * wave2),
                static_cast<Uint8>(82 + 40 * wave1 + 15 * wave2),
                static_cast<Uint8>(45 + 25 * wave1 + 10 * wave2),
                255
            };
            
            SDL_SetRenderDrawColor(renderer, stepColor.r, stepColor.g, stepColor.b, stepColor.a);
            SDL_Rect colorRect = {
                stepRect.x,
                stepRect.y + y,
                stepRect.w,
                1
            };
            SDL_RenderFillRect(renderer, &colorRect);
        }
        
        // Add cloud-like soft highlight on top edge
        for (int highlight = 0; highlight < 3; ++highlight) {
            float alpha = 150 - highlight * 40;
            SDL_Color highlightColor = {
                static_cast<Uint8>(200 + highlight * 20),
                static_cast<Uint8>(120 + highlight * 15),
                static_cast<Uint8>(60 + highlight * 10),
                static_cast<Uint8>(alpha)
            };
            
            SDL_SetRenderDrawColor(renderer, highlightColor.r, highlightColor.g, highlightColor.b, highlightColor.a);
            SDL_Rect highlightRect = {
                stepRect.x,
                stepRect.y + highlight,
                stepRect.w,
                1
            };
            SDL_RenderFillRect(renderer, &highlightRect);
        }
        
        // Add soft shadow on bottom edge
        for (int shadow = 0; shadow < 2; ++shadow) {
            float alpha = 80 - shadow * 30;
            SDL_SetRenderDrawColor(renderer, 40, 25, 10, static_cast<Uint8>(alpha));
            SDL_Rect shadowRect = {
                stepRect.x,
                stepRect.y + stepRect.h - 1 - shadow,
                stepRect.w,
                1
            };
            SDL_RenderFillRect(renderer, &shadowRect);
        }
    }
    
    // Draw artistic outline with gradient
    for (int i = 0; i < 4; ++i) {
        float gradientT = static_cast<float>(i) / 4.0f;
        SDL_Color outlineColor = {
            static_cast<Uint8>(80 + 40 * std::sin(gradientT * 3.14159f)),
            static_cast<Uint8>(40 + 20 * std::cos(gradientT * 3.14159f)),
            static_cast<Uint8>(10 + 10 * std::sin(gradientT * 2.0f * 3.14159f)),
            255
        };
        
        SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
        SDL_Rect outlineRect = {
            stairRect.x - i,
            stairRect.y - i,
            stairRect.w + i * 2,
            stairRect.h + i * 2
        };
        SDL_RenderDrawRect(renderer, &outlineRect);
    }
}
