#ifndef LIGHT_MANAGER_HPP
#define LIGHT_MANAGER_HPP

#include <SDL2/SDL.h>
#include <vector>

struct PointLight {
    float x;
    float y;
    float radius;
    float intensity;
    Uint8 r;
    Uint8 g;
    Uint8 b;
};

class LightManager {
public:
    LightManager();
    ~LightManager();

    bool initialize(SDL_Renderer* renderer, int textureSize);

    int addLight(const PointLight& light);
    void clearLights();
    void setLightPosition(int index, float x, float y);
    void render(SDL_Renderer* renderer, float cameraX, float cameraY);

private:
    SDL_Texture* m_lightTexture;
    int m_textureSize;
    std::vector<PointLight> m_lights;
};

#endif
