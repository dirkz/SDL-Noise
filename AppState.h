#pragma once

#include "stdafx.h"

struct AppState
{
    AppState();

    void Iterate();
    void ClearScreen(DirectX::FXMVECTOR color);

  private:
    void BenchmarkNoises();

    int m_windowWidth = 0;
    int m_windowHeight = 0;

    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_renderer = nullptr;

    SDL_Texture *m_texture = nullptr;
};
