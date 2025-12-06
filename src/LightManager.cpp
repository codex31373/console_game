#include "LightManager.hpp"
#include <cmath>
#include <algorithm>

LightManager::LightManager()
    : m_lightTexture(nullptr)
    , m_textureSize(0) {}

LightManager::~LightManager() {
    if (m_lightTexture) {
        SDL_DestroyTexture(m_lightTexture);
        m_lightTexture = nullptr;
    }
}

bool LightManager::initialize(SDL_Renderer* renderer, int textureSize) {
    m_textureSize = textureSize;

    m_lightTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, textureSize, textureSize);
    if (!m_lightTexture) {
        return false;
    }

    void* pixels = nullptr;
    int pitch = 0;
    if (SDL_LockTexture(m_lightTexture, nullptr, &pixels, &pitch) != 0) {
        SDL_DestroyTexture(m_lightTexture);
        m_lightTexture = nullptr;
        return false;
    }

    unsigned char* data = static_cast<unsigned char*>(pixels);
    float center = static_cast<float>(textureSize) / 2.0f;

    for (int y = 0; y < textureSize; ++y) {
        for (int x = 0; x < textureSize; ++x) {
            float dx = (static_cast<float>(x) - center) / center;
            float dy = (static_cast<float>(y) - center) / center;
            float dist = std::sqrt(dx * dx + dy * dy);

            float t = dist;
            if (t > 1.0f) {
                t = 1.0f;
            }

            float intensity = 1.0f - t;
            intensity = intensity * intensity;

            unsigned char alpha = static_cast<unsigned char>(intensity * 255.0f);

            int offset = y * pitch + x * 4;
            data[offset + 0] = 255;
            data[offset + 1] = 255;
            data[offset + 2] = 255;
            data[offset + 3] = alpha;
        }
    }

    SDL_UnlockTexture(m_lightTexture);
    SDL_SetTextureBlendMode(m_lightTexture, SDL_BLENDMODE_ADD);

    return true;
}

int LightManager::addLight(const PointLight& light) {
    m_lights.push_back(light);
    return static_cast<int>(m_lights.size()) - 1;
}

void LightManager::clearLights() {
    m_lights.clear();
}

void LightManager::setLightPosition(int index, float x, float y) {
    if (index < 0) {
        return;
    }
    if (index >= static_cast<int>(m_lights.size())) {
        return;
    }

    m_lights[static_cast<std::size_t>(index)].x = x;
    m_lights[static_cast<std::size_t>(index)].y = y;
}

void LightManager::render(SDL_Renderer* renderer, float cameraX, float cameraY) {
    if (!m_lightTexture) {
        return;
    }

    for (const auto& light : m_lights) {
        int dstW = static_cast<int>(light.radius * 2.0f);
        int dstH = static_cast<int>(light.radius * 2.0f);

        int screenX = static_cast<int>(light.x - cameraX - light.radius);
        int screenY = static_cast<int>(light.y - cameraY - light.radius);

        SDL_Rect dst = {screenX, screenY, dstW, dstH};

        SDL_SetTextureColorMod(m_lightTexture, light.r, light.g, light.b);
        float intensityClamped = std::max(0.0f, std::min(1.0f, light.intensity));
        unsigned char alpha = static_cast<unsigned char>(intensityClamped * 255.0f);
        SDL_SetTextureAlphaMod(m_lightTexture, alpha);

        SDL_RenderCopy(renderer, m_lightTexture, nullptr, &dst);
    }
}
