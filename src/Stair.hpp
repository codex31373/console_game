#ifndef STAIR_HPP
#define STAIR_HPP

#include "GameObject.hpp"
#include <vector>
#include <SDL2/SDL.h>

class Stair : public GameObject {
public:
    // Direction the stairs are facing (true = right, false = left)
    Stair(float x, float y, float width, float height, bool facingRight = true);
    
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) const override;
    
    bool isFacingRight() const { return m_facingRight; }
    
private:
    bool m_facingRight;
    float m_time = 0.0f;
    std::vector<float> m_shimmerPattern;
    static const int SHIMMER_PATTERN_SIZE = 64;
    
    void generateShimmerPattern();
};

#endif // STAIR_HPP
