#pragma once

struct Perlin
{
    // 6x^5 - 15x^4 + 10^3
    // x * x * x * (x * (6 * x - 15) + 10)
    static inline DirectX::XMVECTOR Fade(DirectX::FXMVECTOR v)
    {
        using namespace DirectX;

        static const DirectX::XMVECTOR V15 = DirectX::XMVectorReplicate(15.f);
        static const DirectX::XMVECTOR V10 = DirectX::XMVectorReplicate(10.f);

        XMVECTOR vv = XMVectorScale(v, 6.f);
        vv = XMVectorSubtract(vv, V15);
        vv = XMVectorMultiply(v, vv);
        vv = XMVectorAdd(vv, V10);
        XMVECTOR v3 = XMVectorMultiply(v, XMVectorMultiply(v, v));
        vv = XMVectorMultiply(v3, vv);

        return vv;
    }

    // a + t * (b - a)
    static inline DirectX::XMVECTOR Lerp(DirectX::FXMVECTOR t, DirectX::FXMVECTOR a,
                                         DirectX::FXMVECTOR b)
    {
        using namespace DirectX;

        XMVECTOR v = XMVectorSubtract(b, a);
        v = XMVectorMultiply(v, t);
        v = XMVectorAdd(a, v);

        return v;
    }

    template <class T> static T Lerp(T t, T a, T b)
    {
        return a + t * b;
    }

    float Noise(float x, float y, float z);

  private:
    int Hash(int x, int y, int z);
    DirectX::XMVECTOR GradientAt(int x, int y, int z);
};
