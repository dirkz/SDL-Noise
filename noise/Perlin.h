#pragma once

struct Perlin
{

    template <class T> T fade(T x)
    {
        // 6x^5 - 15x^4 + 10^3
        return x * x * x * (x * (6 * x - 15) + 10);
    }

    template <class T> T lerp(T t, T a, T b)
    {
        return a + t * b;
    }

    float Noise(float x, float y, float z);
};
