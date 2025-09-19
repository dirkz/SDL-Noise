#pragma once

#include "stdafx.h"

struct ImprovedNoise
{
    template <class T> static T fade(T t)
    {
        return t * t * t * (t * (t * T{6} - T{15}) + T{10});
    }

    template <class T> static T lerp(T t, T a, T b)
    {
        return a + t * (b - a);
    }
};
