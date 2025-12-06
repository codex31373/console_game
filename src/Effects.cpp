#include "Effects.hpp"
#include <cmath>
#include <cstdlib>

Effects::Effects()
    : m_vignetteTexture(nullptr)
    , m_grainTexture(nullptr)
    , m_screenWidth(0)
    , m_screenHeight(0) {}

Effects::~Effects() {
    if (m_vignetteTexture) {
        SDL_DestroyTexture(m_vignetteTexture);
        m_vignetteTexture = nullptr;
    }
    if (m_grainTexture) {
        SDL_DestroyTexture(m_grainTexture);
        m_grainTexture = nullptr;
    }
}

bool Effects::initialize(SDL_Renderer* renderer, int screenWidth, int screenHeight) {
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    m_vignetteTexture = createVignetteTexture(renderer, screenWidth, screenHeight);
    m_grainTexture = createGrainTexture(renderer, 256, 256);

    if (m_vignetteTexture) {
        SDL_SetTextureBlendMode(m_vignetteTexture, SDL_BLENDMODE_BLEND);
    }
    if (m_grainTexture) {
        SDL_SetTextureBlendMode(m_grainTexture, SDL_BLENDMODE_BLEND);
    }

    return m_vignetteTexture != nullptr && m_grainTexture != nullptr;
}

SDL_Texture* Effects::createVignetteTexture(SDL_Renderer* renderer, int width, int height) {
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!texture) {
        return nullptr;
    }

    void* pixels = nullptr;
    int pitch = 0;
    if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0) {
        SDL_DestroyTexture(texture);
        return nullptr;
    }

    unsigned char* dst = static_cast<unsigned char*>(pixels);
    float cx = static_cast<float>(width) * 0.5f;
    float cy = static_cast<float>(height) * 0.5f;
    float maxDist = std::sqrt(cx * cx + cy * cy);
    float innerRadius = 0.5f;
    float strength = 0.7f;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float dx = static_cast<float>(x) - cx;
            float dy = static_cast<float>(y) - cy;
            float dist = std::sqrt(dx * dx + dy * dy) / maxDist;
            float t = (dist - innerRadius) / (1.0f - innerRadius);
            if (t < 0.0f) {
                t = 0.0f;
            }
            if (t > 1.0f) {
                t = 1.0f;
            }
            float alphaF = t * strength;
            unsigned char a = static_cast<unsigned char>(alphaF * 255.0f);

            int offset = y * pitch + x * 4;
            dst[offset + 0] = 0;
            dst[offset + 1] = 0;
            dst[offset + 2] = 0;
            dst[offset + 3] = a;
        }
    }

    SDL_UnlockTexture(texture);
    return texture;
}

SDL_Texture* Effects::createGrainTexture(SDL_Renderer* renderer, int width, int height) {
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!texture) {
        return nullptr;
    }

    void* pixels = nullptr;
    int pitch = 0;
    if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0) {
        SDL_DestroyTexture(texture);
        return nullptr;
    }

    unsigned char* dst = static_cast<unsigned char*>(pixels);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char v = static_cast<unsigned char>(200 + std::rand() % 55);
            unsigned char a = static_cast<unsigned char>(50 + std::rand() % 80);

            int offset = y * pitch + x * 4;
            dst[offset + 0] = v;
            dst[offset + 1] = v;
            dst[offset + 2] = v;
            dst[offset + 3] = a;
        }
    }

    SDL_UnlockTexture(texture);
    return texture;
}

void Effects::applyPostProcessing(SDL_Renderer* renderer, Uint32 ticks) {
    if (!renderer) {
        return;
    }

    if (m_vignetteTexture) {
        SDL_Rect dst = {0, 0, m_screenWidth, m_screenHeight};
        SDL_RenderCopy(renderer, m_vignetteTexture, nullptr, &dst);
    }

    if (m_grainTexture) {
        int texW = 0;
        int texH = 0;
        SDL_QueryTexture(m_grainTexture, nullptr, nullptr, &texW, &texH);

        int offsetX = static_cast<int>((ticks / 7) % static_cast<Uint32>(texW));
        int offsetY = static_cast<int>((ticks / 13) % static_cast<Uint32>(texH));

        SDL_SetTextureAlphaMod(m_grainTexture, 70);

        for (int y = -texH; y < m_screenHeight + texH; y += texH) {
            for (int x = -texW; x < m_screenWidth + texW; x += texW) {
                SDL_Rect dst = {x + offsetX, y + offsetY, texW, texH};
                SDL_RenderCopy(renderer, m_grainTexture, nullptr, &dst);
            }
        }
    }
}
