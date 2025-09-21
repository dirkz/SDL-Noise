#include "AppState.h"

#include "ImprovedNoise.h"
#include "NoiseDX.h"

using namespace DirectX;

constexpr int WindowWidth = 1280;
constexpr int WindowHeight = 800;

static void SetPixel(void *pixels, int pitch, int x, int y, FXMVECTOR rgba)
{
    XMFLOAT4 fRgba;
    XMStoreFloat4(&fRgba, rgba);

    unsigned char r = static_cast<unsigned char>(fRgba.x * 255.f);
    unsigned char g = static_cast<unsigned char>(fRgba.y * 255.f);
    unsigned char b = static_cast<unsigned char>(fRgba.z * 255.f);
    unsigned char a = static_cast<unsigned char>(fRgba.w * 255.f);

    Uint32 color = a + (b << 8) + (g << 16) + (r << 24);

    int lineOffset = pitch * y;
    unsigned char *bytes = reinterpret_cast<unsigned char *>(pixels) + lineOffset;
    Uint32 *row = reinterpret_cast<Uint32 *>(bytes);

    row[x] = color;
}

template <class T>
static SDL_Texture *CreateTexture(SDL_Renderer *renderer, int width, int height, const T &noise)
{
    SDL_Surface *surface = sdl::CreateSurface(width, height, SDL_PIXELFORMAT_RGBA8888);
    sdl::Surface surfaceToDelete{surface};

    int pitch = surface->pitch;

    sdl::LockSurface(surface);

    DirectX::XMVECTORF32 baseColor = DirectX::Colors::CornflowerBlue;

    constexpr float frequency = 1.f / 32.f;
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            float x = static_cast<float>(i) * frequency;
            float y = static_cast<float>(j) * frequency;
            float n = noise(x, y, 0.5f);
            n = (n + 1) / 2;
            DirectX::XMVECTOR scale = DirectX::XMVectorReplicate(n);
            DirectX::XMVECTOR color = DirectX::XMColorModulate(baseColor, scale);
            SetPixel(surface->pixels, pitch, i, j, color);
        }
    }

    sdl::UnlockSurface(surface);

    SDL_Texture *texture = sdl::CreateTextureFromSurface(renderer, surface);

    return texture;
}

template <class N, class T> void RunNoise(const N &noise, T frequency, int width, int height)
{
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            T x = static_cast<T>(i) * frequency;
            T y = static_cast<T>(j) * frequency;
            T n = noise(x, y, 0.5f);
        }
    }
}

template <class T> void Measure(const char *what, const T &fn)
{
    auto begin = std::chrono::high_resolution_clock::now();
    fn();
    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    SDL_Log("*** %s: %d", what, static_cast<int>(elapsed));
}

AppState::AppState() : m_windowWidth{WindowWidth}, m_windowHeight{WindowHeight}
{
    sdl::SetAppMetadata("SDL-Noise", "0.0.1", "com.dirkz.samples.sdl.noise");
    sdl::Init(SDL_INIT_VIDEO);
    sdl::CreateWindowAndRenderer("SDL-Noise", WindowWidth, WindowHeight, 0, &m_window, &m_renderer);

    auto improvedNoiseFloat = [](float x, float y, float z) {
        return ImprovedNoise::Noise(x, y, z);
    };
    auto noiseDX = [](float x, float y, float z) { return NoiseDX::Noise(x, y, z); };

    m_texture1 = CreateTexture(m_renderer, WindowWidth / 2, WindowHeight, improvedNoiseFloat);
    m_texture2 = CreateTexture(m_renderer, WindowWidth / 2, WindowHeight, noiseDX);
}

void AppState::Iterate()
{
    Uint64 millis = sdl::GetTicks();
    double seconds = static_cast<double>(millis) / 1000.;

    float halfWidth = static_cast<float>(m_windowWidth) / 2.f;
    float height = static_cast<float>(m_windowHeight);

    SDL_FRect rect1{.x = 0, .y = 0, .w = halfWidth, .h = height};
    SDL_FRect rect2{.x = halfWidth, .y = 0, .w = halfWidth, .h = height};

    sdl::RenderTexture(m_renderer, m_texture1, nullptr, &rect1);
    sdl::RenderTexture(m_renderer, m_texture2, nullptr, &rect2);

    sdl::RenderPresent(m_renderer);
}

void AppState::ClearScreen(FXMVECTOR color)
{
    XMFLOAT4 fColor;
    XMStoreFloat4(&fColor, color);
    float r = fColor.x;
    float g = fColor.y;
    float b = fColor.z;

    sdl::SetRenderDrawColorFloat(m_renderer, r, g, b, SDL_ALPHA_OPAQUE_FLOAT);
    sdl::RenderClear(m_renderer);
}

void AppState::BenchmarkNoises()
{
    auto improvedNoiseFloat = [](float x, float y, float z) {
        return ImprovedNoise::Noise(x, y, z);
    };
    auto noiseDX = [](float x, float y, float z) { return NoiseDX::Noise(x, y, z); };

    auto improvedNoiseDouble = [](double x, double y, double z) {
        return ImprovedNoise::Noise(x, y, z);
    };

    constexpr float frequency = 1.f / 64;
    constexpr int width = 1024;
    constexpr int height = 1024;

    auto runImprovedNoiseFloat = [&improvedNoiseFloat]() {
        RunNoise(improvedNoiseFloat, frequency, width, height);
    };
    auto runImprovedNoiseDouble = [&improvedNoiseDouble]() {
        RunNoise(improvedNoiseDouble, static_cast<double>(frequency), width, height);
    };
    auto runNoiseDX = [&noiseDX]() { RunNoise(noiseDX, frequency, width, height); };

    Measure("ImprovedNoise<float>", runImprovedNoiseFloat);
    Measure("ImprovedNoise<double>", runImprovedNoiseDouble);
    Measure("NoiseDX", runNoiseDX);
}
