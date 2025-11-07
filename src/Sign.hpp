#ifndef SIGN_HPP
#define SIGN_HPP

#include "GameObject.hpp"
#include <SDL2/SDL.h>
#include <string>
#include <cmath>

class Sign : public GameObject {
public:
    Sign(float x, float y, const std::string& text, const Color& color = {255, 255, 255, 255});
    
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) const override;
    
    void setEffectEnabled(bool enabled) { m_effectEnabled = enabled; }
    void setEffectSpeed(float speed) { m_effectSpeed = speed; }
    void setScale(float scale) { m_scale = scale; }
    void setTextAlignment(float alignment) { m_textAlignment = alignment; }  // 0.0 = left, 0.5 = center, 1.0 = right
    
private:
    std::string m_text;
    bool m_effectEnabled = true;
    float m_effectTime = 0.0f;
    float m_effectSpeed = 2.0f;
    float m_scale = 3.0f;  // Scaling factor for the sign
    float m_textAlignment = 0.0f;  // Text alignment (0.0 = left, 0.5 = center, 1.0 = right)
    
    // 8x8 font data for each character (simplified)
    static constexpr int CHAR_WIDTH = 5;
    static constexpr int CHAR_HEIGHT = 7;
    static constexpr int CHAR_SPACING = 1;
    
    // Simple 5x7 font data (1 = pixel on, 0 = pixel off)
    static const uint8_t FONT[][7];
    
    void drawChar(SDL_Renderer* renderer, char c, float x, float y, float scale, const Color& color) const;
    
    // Helper method to draw a pixel with scaling
    void drawPixel(SDL_Renderer* renderer, float x, float y, float scale, const Color& color) const {
        SDL_FRect pixel = {x, y, scale, scale};
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRectF(renderer, &pixel);
    }
};

#endif // SIGN_HPP
