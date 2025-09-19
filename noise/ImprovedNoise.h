#pragma once

#include "stdafx.h"

struct ImprovedNoise
{
    template <class T> static T Fade(T t)
    {
        return t * t * t * (t * (t * T{6} - T{15}) + T{10});
    }

    template <class T> static T Lerp(T t, T a, T b)
    {
        return a + t * (b - a);
    }
};
