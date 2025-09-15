#pragma once

#include "stdafx.h"

#include "Perlin.h"

struct AppState
{
    AppState();

    void Iterate();

  private:
    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_renderer = nullptr;
    Perlin m_perlinNoise;
};
