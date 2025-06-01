#pragma once

#include <SDL.h>
#include <string>
#include <map>
#include "CTexture.h"

class CTexture;
class CVector2D;

class CTextureManager
{
    SDL_Renderer* pRenderer;

public:
    CTextureManager(SDL_Renderer* renderer)
    {
        pRenderer = renderer;
    }

    bool Load(std::string fileName, std::string id);
    void Add(CTexture* nText, std::string id);

    void CreateCheckboardPattern(CVector2D size, std::string texName);

    // draw
    void Draw(std::string id, int x, int y, int width, int height,
              double angle = 0.0f, SDL_Point* center = nullptr,
              SDL_RendererFlip flip = SDL_FLIP_NONE);

    // draw
    void Draw(CTexture* texture, int x, int y,
              double angle = 0.0f, SDL_Point* center = nullptr,
              SDL_RendererFlip flip = SDL_FLIP_NONE);

    // drawframe
    void DrawFrame(std::string id, int x, int y, int width, int height,
                   int currentRow, int currentFrame, double angle = 0.0f,
                   SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

    void DrawFrame(CTexture* texture, int x, int y,
                   int currentRow, int currentFrame, double angle = 0.0f,
                   SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

    void DrawTile(std::string id, int margin, int spacing,
                  int x, int y, int width, int height, int currentRow,
                  int currentFrame);

    void Remove(std::string id);

    std::map<std::string, CTexture*> m_textureMap;
};

