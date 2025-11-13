#ifndef WATER_HPP
#define WATER_HPP

#include "GameObject.hpp"
#include <vector>
#include <SDL2/SDL.h>

class Water : public GameObject {
public:
    struct WaveLayer {
        float time = 0.0f;
        float speed = 1.0f;
        float height = 1.0f;
        float frequency = 1.0f;
        SDL_Color colors[2];
    };

    Water(float x, float y, float width, float height);
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) const override;
    
private:
    void generateFoamPattern();
    
    float m_time = 0.0f;
    std::vector<float> m_foamPattern;
    std::vector<WaveLayer> m_waveLayers;
    static const int WAVE_POINTS = 64;
    static const int FOAM_PATTERN_SIZE = 128;
};

#endif // WATER_HPP
