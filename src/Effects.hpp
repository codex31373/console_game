#ifndef EFFECTS_HPP
#define EFFECTS_HPP

#include <SDL2/SDL.h>

class Effects {
public:
    Effects();
    ~Effects();

    bool initialize(SDL_Renderer* renderer, int screenWidth, int screenHeight);

    void applyPostProcessing(SDL_Renderer* renderer, Uint32 ticks);

private:
    SDL_Texture* m_vignetteTexture;
    SDL_Texture* m_grainTexture;
    int m_screenWidth;
    int m_screenHeight;

    SDL_Texture* createVignetteTexture(SDL_Renderer* renderer, int width, int height);
    SDL_Texture* createGrainTexture(SDL_Renderer* renderer, int width, int height);
};

#endif
