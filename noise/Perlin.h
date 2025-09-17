#pragma once

struct Perlin
{
    Perlin();

    template <class T> T Lerp(T t, T a, T b)
    {
        return a + t * b;
    }

    float Noise(float x, float y, float z);

  private:
    int Hash(int x, int y, int z);
    DirectX::XMVECTOR GradientAt(int x, int y, int z);
};
