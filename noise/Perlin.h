#pragma once

struct Perlin
{
    Perlin();

    template <class T> T Lerp(T t, T a, T b)
    {
        return a + t * b;
    }

    float Noise(float x, float y, float z);

    /// 6x^5 - 15x^4 + 10^3
    inline DirectX::XMVECTOR Fade(DirectX::FXMVECTOR v)
    {
        using namespace DirectX;

        static const XMVECTOR V15 = XMVectorReplicate(15.f);
        static const XMVECTOR V10 = XMVectorReplicate(10.f);

        XMVECTOR vv = XMVectorScale(v, 6.f);
        vv = XMVectorSubtract(vv, V15);
        vv = XMVectorMultiply(v, vv);
        vv = XMVectorAdd(vv, V10);
        XMVECTOR v3 = XMVectorMultiply(v, XMVectorMultiply(v, v));
        vv = XMVectorMultiply(v3, vv);

        return vv;
    }

  private:
    int Hash(int x, int y, int z);
    DirectX::XMVECTOR GradientAt(int x, int y, int z);
};
