#pragma once

#include "stdafx.h"

#include "ImprovedNoise.h"

struct AppState
{
    AppState();

    void Iterate();
    void ClearScreen(DirectX::FXMVECTOR color);

  private:
    void CreateTexture();

    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_renderer = nullptr;
    SDL_Surface *m_surface = nullptr;
    SDL_Texture *m_texture = nullptr;

    ImprovedNoise m_noise;
};
