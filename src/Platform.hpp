#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "GameObject.hpp"

class Platform : public GameObject {
public:
    Platform(float x, float y, float width, const Color& color = {139, 69, 19, 255});
    
    void render(SDL_Renderer* renderer) const override;
    
    // Platform-specific methods if needed
    bool canStandOn(float playerBottom) const;
    
private:
    static constexpr float HEIGHT = 30.0f; // Half of character height
};

#endif // PLATFORM_HPP
