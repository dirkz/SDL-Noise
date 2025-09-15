#include "AppState.h"

AppState::AppState()
{
    sdl::CreateWindowAndRenderer("SDL-Noise", 1280, 800, 0, &m_window, &m_renderer);
}
