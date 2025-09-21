#pragma once

#include "stdafx.h"

struct NoiseDX
{
    template <class T> static T Fade(T t)
    {
        return t * t * t * (t * (t * T{6} - T{15}) + T{10});
    }

    template <class T> static T Lerp(T t, T a, T b)
    {
        return a + t * (b - a);
    }

    static DirectX::XMVECTOR Fade(DirectX::FXMVECTOR t);

    // Original implementation (improved noise)
    static float Grad0(int hash, float x, float y, float z);

    // Will use either the original implementation, or the pure vector one
    static float Grad0(int hash, DirectX::FXMVECTOR v);

    // Pure vector gradients
    static float Grad(int hash, DirectX::FXMVECTOR v);

    // Pure vector gradients, with original interface
    static float Grad(int hash, float x, float y, float z);

    static float Noise(float x, float y, float z);
};
