#pragma once

#include "stdafx.h"

struct AppState
{
    AppState();

    void Iterate();

  private:
    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_renderer = nullptr;
};
