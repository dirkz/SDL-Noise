#include "AppState.h"

using namespace DirectX;

AppState::AppState()
{
    constexpr int windowWidth = 1280;
    constexpr int windowHeight = 800;

    sdl::SetAppMetadata("SDL-Noise", "0.0.1", "com.dirkz.samples.sdl.noise");
    sdl::Init(SDL_INIT_VIDEO);
    sdl::CreateWindowAndRenderer("SDL-Noise", windowWidth, windowHeight, 0, &m_window, &m_renderer);

    m_surface = sdl::CreateSurface(windowWidth, windowHeight, SDL_PIXELFORMAT_RGBA8888);

    CreateTexture();
}

void AppState::Iterate()
{
    Uint64 millis = sdl::GetTicks();
    double seconds = static_cast<double>(millis) / 1000.;

    XMVECTORF32 color = DirectX::Colors::CornflowerBlue;
    const float *fColor = color;
    float r = fColor[0];
    float g = fColor[1];
    float b = fColor[2];

    sdl::SetRenderDrawColorFloat(m_renderer, r, g, b, SDL_ALPHA_OPAQUE_FLOAT);
    // sdl::RenderClear(m_renderer);

    sdl::RenderTexture(m_renderer, m_texture, nullptr, nullptr);

    sdl::RenderPresent(m_renderer);
}

static void SetPixel(void *pixels, int pitch, int x, int y, FXMVECTOR rgba)
{
    XMFLOAT4 fRgba;
    XMStoreFloat4(&fRgba, rgba);

    unsigned char r = static_cast<unsigned char>(fRgba.x * 255.f);
    unsigned char g = static_cast<unsigned char>(fRgba.y * 255.f);
    unsigned char b = static_cast<unsigned char>(fRgba.z * 255.f);
    unsigned char a = static_cast<unsigned char>(fRgba.w * 255.f);

    int offset = pitch * y + x * 4;

    unsigned char *bytes = reinterpret_cast<unsigned char *>(pixels);
    bytes[offset + 0] = r;
    bytes[offset + 1] = g;
    bytes[offset + 2] = b;
    bytes[offset + 3] = a;
}

void AppState::CreateTexture()
{
    int width = m_surface->w;
    int height = m_surface->h;
    int pitch = m_surface->pitch;

    sdl::LockSurface(m_surface);

    XMVECTORF32 baseColor = DirectX::Colors::CornflowerBlue;

    constexpr float frequency = 1.f / 6.f;
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            float x = static_cast<float>(i) * frequency;
            float y = static_cast<float>(j) * frequency;
            float n = m_perlinNoise.Noise(x, y, 0.5f);
            n = (n + 1) / 2;
            XMVECTOR scale = XMVectorReplicate(n);
            XMVECTOR color = XMColorModulate(baseColor, scale);
            SetPixel(m_surface->pixels, pitch, i, j, baseColor);
        }
    }

    sdl::UnlockSurface(m_surface);

    m_texture = sdl::CreateTextureFromSurface(m_renderer, m_surface);
}
