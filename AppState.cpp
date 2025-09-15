#include "AppState.h"

using namespace DirectX;

AppState::AppState()
{
    sdl::SetAppMetadata("SDL-Noise", "0.0.1", "com.dirkz.samples.sdl.noise");
    sdl::Init(SDL_INIT_VIDEO);
    sdl::CreateWindowAndRenderer("SDL-Noise", 1280, 800, 0, &m_window, &m_renderer);
}

void AppState::Iterate()
{
    Uint64 millis = sdl::GetTicks();
    double seconds = static_cast<double>(millis) / 1000.;

    XMVECTORF32 color = DirectX::Colors::CornflowerBlue;
    const float *floats = color;
    float r = floats[0];
    float g = floats[1];
    float b = floats[2];

    sdl::SetRenderDrawColorFloat(m_renderer, r, g, b, SDL_ALPHA_OPAQUE_FLOAT);
    sdl::RenderClear(m_renderer);
    sdl::RenderPresent(m_renderer);
}
