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

    static double Grad(int hash, double x, double y, double z);

    static double Noise(double x, double y, double z);
};
