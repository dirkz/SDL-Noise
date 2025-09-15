#include "AppState.h"

AppState::AppState()
{
    sdl::SetAppMetadata("SDL-Noise", "0.0.1", "com.dirkz.samples.sdl.noise");
    sdl::Init(SDL_INIT_VIDEO);
    sdl::CreateWindowAndRenderer("SDL-Noise", 1280, 800, 0, &m_window, &m_renderer);
}
