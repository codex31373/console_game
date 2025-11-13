#include "Water.hpp"
#include "Common.hpp"
#include <cmath>
#include <random>
#include <algorithm>
#include <SDL2/SDL.h>

Water::Water(float x, float y, float width, float height)
    : GameObject(x, y, width, height, {25, 100, 160, 180}) {  // Deeper blue with more transparency
    // Initialize wave layers with different properties
    WaveLayer layer1, layer2, layer3;
    
    // Main slow waves - increased size and adjusted for better visibility
    layer1.time = 0.0f;
    layer1.speed = 0.15f;  // Even slower speed
    layer1.height = 20.0f;  // Much more pronounced waves
    layer1.frequency = 0.25f;  // Wider waves
    layer1.colors[0] = {20, 100, 200, 180};
    layer1.colors[1] = {40, 140, 230, 200};
    
    // Secondary waves - medium speed
    layer2.time = 50.0f;
    layer2.speed = 0.3f;
    layer2.height = 10.0f;  // Increased height
    layer2.frequency = 0.5f;  // Slightly wider
    layer2.colors[0] = {30, 120, 210, 160};
    layer2.colors[1] = {50, 150, 240, 180};
    
    // High frequency waves - fine detail
    layer3.time = 100.0f;
    layer3.speed = 0.45f;
    layer3.height = 6.0f;  // Increased height for better visibility
    layer3.frequency = 1.0f;  // Slightly less frequent
    layer3.colors[0] = {40, 140, 220, 140};
    layer3.colors[1] = {60, 170, 250, 160};
    
    m_waveLayers.push_back(layer1);
    m_waveLayers.push_back(layer2);
    m_waveLayers.push_back(layer3);
    
    // Generate random foam pattern
    generateFoamPattern();
}

void Water::generateFoamPattern() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    
    m_foamPattern.resize(FOAM_PATTERN_SIZE);
    for (int i = 0; i < FOAM_PATTERN_SIZE; ++i) {
        float t = static_cast<float>(i) / FOAM_PATTERN_SIZE;
        float noise = 0.5f + 0.5f * std::sin(t * 15.0f) * std::cos(t * 10.0f);
        m_foamPattern[i] = 0.3f + 0.7f * noise * dist(gen);
    }
}

void Water::update(float deltaTime) {
    m_time += deltaTime;
    
    // Update each wave layer
    for (auto& layer : m_waveLayers) {
        layer.time += deltaTime * layer.speed;
    }
}

void Water::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    // Draw base water color - extended to the left to prevent edge artifacts
    SDL_SetRenderDrawColor(renderer, 30, 100, 200, 200);
    SDL_Rect waterRect = {
        -100,  // Extend 100 pixels to the left
        static_cast<int>(m_y - m_cameraOffsetY),
        static_cast<int>(m_width + 100),  // Extend width to cover the offset
        static_cast<int>(m_height)
    };
    SDL_RenderFillRect(renderer, &waterRect);
    
    // Draw wave layers from bottom to top
    for (const auto& layer : m_waveLayers) {
        std::vector<SDL_Vertex> vertices;
        std::vector<int> indices;
        
        // Add bottom-left and bottom-right vertices for the water body - extended to the left
        SDL_Vertex bottomLeft = {
            {-100.0f, static_cast<float>(m_y + m_height - m_cameraOffsetY)},
            {layer.colors[0].r, layer.colors[0].g, layer.colors[0].b, 180},
            {0, 0}
        };
        
        SDL_Vertex bottomRight = {
            {static_cast<float>(m_width + 100), static_cast<float>(m_y + m_height - m_cameraOffsetY)},
            {layer.colors[0].r, layer.colors[0].g, layer.colors[0].b, 180},
            {0, 0}
        };
        
        // Add wave points along the top edge
        for (int i = 0; i < WAVE_POINTS; ++i) {
            float t = static_cast<float>(i) / (WAVE_POINTS - 1);
            // Extend the wave points beyond the left edge to prevent diagonal artifacts
            float x = t * (m_width + 200) - 100;  // Start 100px left of the screen
            
            // Calculate wave height using multiple frequencies for natural look
            float wave = 0.0f;
            wave += std::sin(t * 10.0f + layer.time * layer.frequency) * layer.height;
            wave += std::sin(t * 5.0f + layer.time * layer.frequency * 0.7f) * layer.height * 0.5f;
            
            // Add some noise for more natural look
            float noise = m_foamPattern[(i + static_cast<int>(layer.time * 20.0f)) % FOAM_PATTERN_SIZE];
            wave += (noise - 0.5f) * 2.0f;
            
            SDL_Vertex vertex = {
                {x, m_y - m_cameraOffsetY + wave},
                {layer.colors[1].r, layer.colors[1].g, layer.colors[1].b, 
                 static_cast<Uint8>(200 * (0.7f + 0.3f * (wave + layer.height) / (layer.height * 2.0f)))},
                {0, 0}
            };
            
            vertices.push_back(vertex);
            
            // Create triangles for the water surface - with extended coverage
            if (i > 0) {
                int idx = vertices.size() - 1;
                // Only create triangles if both points are within the visible area or just outside
                if (x > -200 && x < m_width + 200) {
                    // Triangle 1: bottom-left, previous top, current top
                    vertices.push_back(bottomLeft);
                    indices.push_back(vertices.size() - 1);
                    indices.push_back(idx - 1);
                    indices.push_back(idx);
                    
                    // Triangle 2: bottom-right, bottom-left, current top
                    vertices.push_back(bottomRight);
                    indices.push_back(vertices.size() - 1);
                    indices.push_back(vertices.size() - 3);
                    indices.push_back(idx);
                }
            }
        }
        
        // Draw the water surface with the calculated vertices and indices
        if (!vertices.empty() && !indices.empty()) {
            SDL_RenderGeometry(renderer, nullptr, vertices.data(), vertices.size(), 
                             indices.data(), indices.size());
        }
    }
}
