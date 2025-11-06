#ifndef STAIR_HPP
#define STAIR_HPP

#include "GameObject.hpp"

class Stair : public GameObject {
public:
    // Direction the stairs are facing (true = right, false = left)
    Stair(float x, float y, float width, float height, bool facingRight = true);
    
    void render(SDL_Renderer* renderer) const override;
    
    bool isFacingRight() const { return m_facingRight; }
    
private:
    bool m_facingRight;
};

#endif // STAIR_HPP
