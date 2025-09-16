#include "Perlin.h"

constexpr int GridLength = 256;

float Perlin::Noise(float x, float y, float z)
{
    float xAbs = std::abs(x);
    float yAbs = std::abs(y);
    float zAbs = std::abs(z);

    int x0 = static_cast<int>(xAbs) % GridLength;
    int y0 = static_cast<int>(yAbs) % GridLength;
    int z0 = static_cast<int>(zAbs) % GridLength;

    float u = xAbs - x0;
    float v = yAbs - y0;
    float w = zAbs - z0;

    float uSmooth = Fade(u);
    float vSmooth = Fade(v);
    float wSmooth = Fade(w);

    return 0.f;
}
