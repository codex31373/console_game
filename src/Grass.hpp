#ifndef GRASS_HPP
#define GRASS_HPP

#include "GameObject.hpp"
#include <vector>
#include <SDL2/SDL.h>

class Grass : public GameObject {
public:
    struct GrassBlade {
        float x;
        float baseY;
        float height;
        float angle;  // Current angle from vertical
        float targetAngle;  // Target angle for smooth animation
        float swayOffset;  // Individual sway timing offset
        SDL_Color color;
        float layerDepth;  // Which layer this blade belongs to (0.0 = back, 1.0 = front)
    };
    
    struct GrassLayer {
        std::vector<GrassBlade> blades;
        float time = 0.0f;
        float speed = 1.0f;
        float height = 1.0f;
        float frequency = 1.0f;
        SDL_Color colors[2];
        float depth;  // Layer depth for rendering order
    };
    
    Grass(float x, float y, float width, float height);
    
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) const override;
    
    // Called when player steps near this grass
    void reactToStep(float stepX, float stepY, float stepForce = 1.0f);
    
private:
    std::vector<GrassLayer> m_grassLayers;
    float m_time = 0.0f;
    static const int GRASS_BLADES_PER_METER = 20;
    static const int LAYER_COUNT = 3;
    
    void generateGrassLayers();
    float calculateWindEffect(float time, float swayOffset, float speed, float frequency) const;
};

#endif // GRASS_HPP
