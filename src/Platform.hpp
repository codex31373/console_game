#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "GameObject.hpp"
#include <vector>
#include <SDL2/SDL.h>

class Platform : public GameObject {
public:
    Platform(float x, float y, float width, const Color& color = {139, 69, 19, 255});
    
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) const override;
    
    // Platform-specific methods if needed
    bool canStandOn(float playerBottom) const;
    
private:
    static constexpr float HEIGHT = 30.0f; // Half of character height
    float m_time = 0.0f;
    std::vector<float> m_texturePattern;
    static const int TEXTURE_PATTERN_SIZE = 128;
    
    void generateTexturePattern();
};

#endif // PLATFORM_HPP
