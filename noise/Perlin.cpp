#include "Perlin.h"

constexpr int GridLength = 256;

float Perlin::Noise(float x, float y, float z)
{
    int x0 = static_cast<int>(x) % GridLength;
    int y0 = static_cast<int>(y) % GridLength;
    int z0 = static_cast<int>(z) % GridLength;

    return 0.f;
}
