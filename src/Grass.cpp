#include "Grass.hpp"
#include <cmath>
#include <random>
#include <algorithm>

Grass::Grass(float x, float y, float width, float height)
    : GameObject(x, y, width, height, {34, 139, 34, 255}) {  // Forest green
    generateGrassLayers();
}

void Grass::generateGrassLayers() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> heightDist(10.0f, 30.0f);
    std::uniform_real_distribution<float> swayDist(0.0f, 6.28f);
    std::uniform_real_distribution<float> bendDist(-0.1f, 0.1f);
    std::uniform_real_distribution<float> colorDist(0.7f, 1.0f);
    
    // Create 3 distinct layers like water waves
    for (int layer = 0; layer < LAYER_COUNT; ++layer) {
        GrassLayer grassLayer;
        
        // Set layer properties similar to water wave layers
        grassLayer.time = layer * 25.0f;  // Stagger initial time
        grassLayer.depth = static_cast<float>(layer) / LAYER_COUNT;
        
        if (layer == 0) {
            // Back layer - shorter, slower, darker
            grassLayer.speed = 0.4f;
            grassLayer.height = 0.7f;
            grassLayer.frequency = 0.8f;
            grassLayer.colors[0] = {20, 80, 20, 180};  // Dark green
            grassLayer.colors[1] = {30, 100, 30, 160};
        } else if (layer == 1) {
            // Middle layer - medium height/speed, medium green
            grassLayer.speed = 0.6f;
            grassLayer.height = 0.85f;
            grassLayer.frequency = 1.0f;
            grassLayer.colors[0] = {34, 120, 34, 190};  // Forest green
            grassLayer.colors[1] = {50, 140, 50, 170};
        } else {
            // Front layer - taller, faster, brighter
            grassLayer.speed = 0.8f;
            grassLayer.height = 1.0f;
            grassLayer.frequency = 1.2f;
            grassLayer.colors[0] = {60, 160, 60, 200};  // Light green
            grassLayer.colors[1] = {80, 180, 80, 180};
        }
        
        // Generate grass blades for this layer
        int bladeCount = static_cast<int>(m_width * GRASS_BLADES_PER_METER / 100.0f);
        bladeCount = static_cast<int>(bladeCount * (0.6f + grassLayer.depth * 0.4f));  // Front layers have more blades
        
        for (int i = 0; i < bladeCount; ++i) {
            GrassBlade blade;
            blade.x = (static_cast<float>(i) / bladeCount) * m_width;
            blade.baseY = m_y + m_height - 2.0f + grassLayer.depth * 3.0f;  // Front layers start slightly higher
            blade.height = heightDist(gen) * grassLayer.height;
            blade.angle = bendDist(gen);
            blade.targetAngle = blade.angle;
            blade.swayOffset = swayDist(gen);
            blade.layerDepth = grassLayer.depth;
            
            // Color based on layer with variation
            float colorVariation = colorDist(gen);
            float colorMix = (static_cast<float>(rand()) / RAND_MAX);
            
            blade.color = {
                static_cast<Uint8>(grassLayer.colors[0].r * (1.0f - colorMix) + grassLayer.colors[1].r * colorMix),
                static_cast<Uint8>(grassLayer.colors[0].g * (1.0f - colorMix) + grassLayer.colors[1].g * colorMix),
                static_cast<Uint8>(grassLayer.colors[0].b * (1.0f - colorMix) + grassLayer.colors[1].b * colorMix),
                static_cast<Uint8>(grassLayer.colors[0].a * colorVariation)
            };
            
            grassLayer.blades.push_back(blade);
        }
        
        m_grassLayers.push_back(grassLayer);
    }
}

void Grass::update(float deltaTime) {
    m_time += deltaTime;
    
    // Update each layer independently like water waves
    for (auto& layer : m_grassLayers) {
        layer.time += deltaTime * layer.speed;
        
        // Update each grass blade in the layer
        for (auto& blade : layer.blades) {
            // Natural wind effect for this layer
            float wind = calculateWindEffect(layer.time, blade.swayOffset, layer.speed, layer.frequency);
            
            // Smooth animation towards target angle
            float angleDiff = blade.targetAngle - blade.angle;
            blade.angle += angleDiff * deltaTime * 5.0f;
            
            // Add layer-specific wind
            blade.angle += wind * 0.15f * (1.0f + blade.layerDepth * 0.5f);  // Front layers respond more to wind
            
            // Gradually return to upright position
            blade.targetAngle *= (1.0f - deltaTime * 2.0f);
            
            // Clamp angle to prevent extreme bending
            blade.angle = std::max(-0.9f, std::min(0.9f, blade.angle));
        }
    }
}

float Grass::calculateWindEffect(float time, float swayOffset, float speed, float frequency) const {
    // Create layered wind patterns similar to water waves
    float wind = 0.0f;
    
    // Primary wind wave - gentle, slow movement
    wind += std::sin(time * 0.6f * frequency + swayOffset) * 0.12f * speed;
    
    // Secondary wave - faster, smaller ripples
    wind += std::sin(time * 1.8f * frequency + swayOffset * 2.1f) * 0.06f * speed;
    
    // Tertiary wave - very fast, fine detail
    wind += std::sin(time * 3.2f * frequency + swayOffset * 0.8f) * 0.03f * speed;
    
    // Occasional gusts - random stronger movements
    if (static_cast<int>(time * 0.3f + swayOffset * 10) % 7 == 0) {
        wind += std::sin(time * 8.0f * frequency + swayOffset) * 0.08f * speed;
    }
    
    // Add some randomness for natural feel
    wind += (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 0.01f;
    
    return wind;
}

void Grass::reactToStep(float stepX, float stepY, float stepForce) {
    // Find grass blades near the step in all layers
    for (auto& layer : m_grassLayers) {
        for (auto& blade : layer.blades) {
            float bladeWorldX = m_x + blade.x;
            float bladeWorldY = blade.baseY;
            
            // Calculate distance from step to blade
            float distance = std::sqrt(
                std::pow(bladeWorldX - stepX, 2) + 
                std::pow(bladeWorldY - stepY, 2)
            );
            
            // Apply force if close enough (front layers have larger reaction radius)
            float reactionRadius = 40.0f + blade.layerDepth * 20.0f;
            if (distance < reactionRadius) {
                // Calculate force based on distance (closer = stronger)
                float force = stepForce * (1.0f - distance / reactionRadius);
                force *= (1.0f + blade.layerDepth * 0.5f);  // Front layers react more strongly
                
                // Determine bend direction based on relative position
                float direction = (bladeWorldX < stepX) ? -1.0f : 1.0f;
                
                // Apply bending force
                blade.targetAngle += direction * force * 0.6f;
            }
        }
    }
}

void Grass::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    // Render layers from back to front (like water waves)
    for (int layerIndex = 0; layerIndex < LAYER_COUNT; ++layerIndex) {
        const auto& layer = m_grassLayers[layerIndex];
        
        // Draw each grass blade in this layer
        for (const auto& blade : layer.blades) {
            // Calculate blade tip position based on angle
            float tipX = blade.x + std::sin(blade.angle) * blade.height;
            float tipY = blade.baseY - std::cos(blade.angle) * blade.height;
            
            // Calculate mid-point for artistic curve
            float midX = blade.x + std::sin(blade.angle * 0.5f) * (blade.height * 0.6f);
            float midY = blade.baseY - std::cos(blade.angle * 0.5f) * (blade.height * 0.6f);
            
            // Draw grass blade with layer-specific thickness
            float thickness = 2.0f + (blade.height / 20.0f) * (1.2f + blade.layerDepth * 0.6f);  // Increased base thickness
            
            // Draw blade as connected segments
            for (int i = -static_cast<int>(thickness); i <= static_cast<int>(thickness); ++i) {
                float offset = i * 0.5f;  // Increased offset for wider blades
                
                // Bottom to middle segment
                SDL_SetRenderDrawColor(renderer, blade.color.r, blade.color.g, blade.color.b, blade.color.a);
                SDL_RenderDrawLine(
                    renderer,
                    static_cast<int>(m_x - m_cameraOffsetX + blade.x + offset),
                    static_cast<int>(blade.baseY - m_cameraOffsetY),
                    static_cast<int>(m_x - m_cameraOffsetX + midX + offset),
                    static_cast<int>(midY - m_cameraOffsetY)
                );
                
                // Middle to tip segment
                SDL_RenderDrawLine(
                    renderer,
                    static_cast<int>(m_x - m_cameraOffsetX + midX + offset),
                    static_cast<int>(midY - m_cameraOffsetY),
                    static_cast<int>(m_x - m_cameraOffsetX + tipX + offset),
                    static_cast<int>(tipY - m_cameraOffsetY)
                );
            }
            
            // Draw artistic blade tip with layer-specific glow
            SDL_Color tipColor = {
                static_cast<Uint8>(std::min(255.0f, blade.color.r + 30.0f + blade.layerDepth * 20.0f)),
                static_cast<Uint8>(std::min(255.0f, blade.color.g + 50.0f + blade.layerDepth * 30.0f)),
                static_cast<Uint8>(std::min(255.0f, blade.color.b + 20.0f + blade.layerDepth * 15.0f)),
                static_cast<Uint8>(blade.color.a * (0.6f + blade.layerDepth * 0.2f))
            };
            
            // Draw glowing tip (front layers have more prominent glow)
            int glowRadius = 1 + static_cast<int>(blade.layerDepth * 2);
            for (int r = glowRadius; r >= 0; --r) {
                Uint8 alpha = tipColor.a * (glowRadius + 1 - r) / (glowRadius + 1);
                SDL_SetRenderDrawColor(renderer, tipColor.r, tipColor.g, tipColor.b, alpha);
                SDL_Rect tipRect = {
                    static_cast<int>(m_x - m_cameraOffsetX + tipX - r),
                    static_cast<int>(tipY - m_cameraOffsetY - r),
                    r * 2 + 1,
                    r * 2 + 1
                };
                SDL_RenderFillRect(renderer, &tipRect);
            }
            
            // Add subtle highlight along the blade edge (more prominent in front layers)
            if (blade.angle > 0.05f && blade.layerDepth > 0.3f) {
                SDL_Color highlightColor = {
                    static_cast<Uint8>(std::min(255.0f, blade.color.r + 25.0f)),
                    static_cast<Uint8>(std::min(255.0f, blade.color.g + 35.0f)),
                    static_cast<Uint8>(std::min(255.0f, blade.color.b + 20.0f)),
                    static_cast<Uint8>(80.0f * blade.layerDepth)
                };
                
                SDL_SetRenderDrawColor(renderer, highlightColor.r, highlightColor.g, highlightColor.b, highlightColor.a);
                SDL_RenderDrawLine(
                    renderer,
                    static_cast<int>(m_x - m_cameraOffsetX + blade.x - 0.5f),
                    static_cast<int>(blade.baseY - m_cameraOffsetY),
                    static_cast<int>(m_x - m_cameraOffsetX + tipX - 0.5f),
                    static_cast<int>(tipY - m_cameraOffsetY)
                );
            }
        }
    }
}
