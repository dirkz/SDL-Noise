#pragma once

struct Perlin
{
    Perlin();

    template <class T> T Fade(T x)
    {
        // 6x^5 - 15x^4 + 10^3
        return x * x * x * (x * (6 * x - 15) + 10);
    }

    template <class T> T Lerp(T t, T a, T b)
    {
        return a + t * b;
    }

    float Noise(float x, float y, float z);

  private:
    void InitGradients();
    int Hash(int x, int y, int z);

    std::array<DirectX::XMVECTOR, 16> m_gradients;
};
