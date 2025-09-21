#include "NoiseDX.h"

#include "ImprovedNoise.h"

namespace NoiseDX
{

using namespace DirectX;

constexpr bool UseOriginalGradients = false;

constexpr float PointW = 1.f;
constexpr float VectorW = 0.f;
constexpr float GradientPlus = +1.f;
constexpr float GradientMinus = -1.f;

static const std::array<XMVECTOR, 16> Gradients{
    XMVectorSet(0, GradientPlus, GradientPlus, VectorW),
    XMVectorSet(0, GradientMinus, GradientMinus, VectorW),
    XMVectorSet(0, GradientPlus, GradientMinus, VectorW),
    XMVectorSet(0, GradientMinus, GradientPlus, VectorW),
    XMVectorSet(GradientPlus, 0, GradientPlus, VectorW),
    XMVectorSet(GradientMinus, 0, GradientMinus, VectorW),
    XMVectorSet(GradientPlus, 0, GradientMinus, VectorW),
    XMVectorSet(GradientMinus, 0, GradientPlus, VectorW),
    XMVectorSet(GradientPlus, GradientPlus, 0, VectorW),
    XMVectorSet(GradientMinus, GradientMinus, 0, VectorW),
    XMVectorSet(GradientPlus, GradientMinus, 0, VectorW),
    XMVectorSet(GradientMinus, GradientPlus, 0, VectorW),
};

DirectX::XMVECTOR Fade(DirectX::FXMVECTOR t)
{
    // t * T{6}
    XMVECTOR v = XMVectorMultiply(t, XMVectorReplicate(6));

    // t * T{6} - T{15}
    v = XMVectorSubtract(v, XMVectorReplicate(15));

    // t * (t * T{6} - T{15})
    v = XMVectorMultiply(t, v);

    // t * (t * T{6} - T{15}) + T{10}
    v = XMVectorAdd(v, XMVectorReplicate(10));

    v = XMVectorMultiply(v, t);
    v = XMVectorMultiply(v, t);
    v = XMVectorMultiply(v, t);

    return v;
}

float Grad(int hash, DirectX::FXMVECTOR v)
{
    int h = hash & 15;
    XMVECTOR gradient = Gradients[h];

    XMVECTOR d = XMVector3Dot(gradient, v);
    float g = XMVectorGetX(d);

    return g;
}

float Grad(int hash, float x, float y, float z)
{
    XMVECTOR point =
        XMVectorSet(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), PointW);

    return Grad(hash, point);
}

float Grad0(int hash, float x, float y, float z)
{
    int h = hash & 15;       // CONVERT LO 4 BITS OF HASH CODE
    float u = h < 8 ? x : y, // INTO 12 GRADIENT DIRECTIONS.
        v = h < 4                ? y
            : h == 12 || h == 14 ? x
                                 : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float Grad0(int hash, DirectX::FXMVECTOR v)
{
    if (UseOriginalGradients)
    {
        XMFLOAT3 fs;
        XMStoreFloat3(&fs, v);

        return Grad0(hash, fs.x, fs.y, fs.z);
    }
    else
    {
        return Grad(hash, v);
    }
}

float Noise(float x, float y, float z)
{
    XMVECTOR p =
        XMVectorSet(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), PointW);

    XMVECTOR pFloor = XMVectorFloor(p);
    XMFLOAT3 fFloor;
    XMStoreFloat3(&fFloor, pFloor);

    int X = static_cast<int>(fFloor.x) & 255, // FIND UNIT CUBE THAT
        Y = static_cast<int>(fFloor.y) & 255, // CONTAINS POINT.
        Z = static_cast<int>(fFloor.z) & 255;

    // FIND RELATIVE X,Y,Z OF POINT IN CUBE.
    p = XMVectorSubtract(p, pFloor);

    XMVECTOR faded = Fade(p);
    XMFLOAT3 fFaded;
    XMStoreFloat3(&fFaded, faded);

    float u = fFaded.x;
    float v = fFaded.y;
    float w = fFaded.z;

    int A = Improved::P[X] + Y, AA = Improved::P[A] + Z,
        AB = Improved::P[A + 1] + Z, // HASH COORDINATES OF
        B = Improved::P[X + 1] + Y, BA = Improved::P[B] + Z,
        BB = Improved::P[B + 1] + Z; // THE 8 CUBE CORNERS,

    const XMVECTOR MinusX = XMVectorSet(-1, 0, 0, 0);
    const XMVECTOR MinusY = XMVectorSet(0, -1, 0, 0);
    const XMVECTOR MinusXY = XMVectorSet(-1, -1, 0, 0);
    const XMVECTOR MinusZ = XMVectorSet(0, 0, -1, 0);
    const XMVECTOR MinusXZ = XMVectorSet(-1, 0, -1, 0);
    const XMVECTOR MinusYZ = XMVectorSet(0, -1, -1, 0);
    const XMVECTOR MinusXYZ = XMVectorSet(-1, -1, -1, 0);

    return Improved::Lerp(
        w,
        Improved::Lerp(v,
                       Improved::Lerp(u, Grad0(Improved::P[AA], p),                      // AND ADD
                                      Grad0(Improved::P[BA], XMVectorAdd(p, MinusX))),   // BLENDED
                       Improved::Lerp(u, Grad0(Improved::P[AB], XMVectorAdd(p, MinusY)), // RESULTS
                                      Grad0(Improved::P[BB], XMVectorAdd(p, MinusXY)))), // FROM  8
        Improved::Lerp(
            v,
            Improved::Lerp(u, Grad0(Improved::P[AA + 1], XMVectorAdd(p, MinusZ)), // CORNERS
                           Grad0(Improved::P[BA + 1], XMVectorAdd(p, MinusXZ))),  // OF CUBE
            Improved::Lerp(u, Grad0(Improved::P[AB + 1], XMVectorAdd(p, MinusYZ)),
                           Grad0(Improved::P[BB + 1], XMVectorAdd(p, MinusXYZ)))));
}

} // namespace NoiseDX