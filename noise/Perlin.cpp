#include "Perlin.h"

using namespace DirectX;

static const XMVECTOR V15 = XMVectorReplicate(15.f);
static const XMVECTOR V10 = XMVectorReplicate(10.f);

// 6x^5 - 15x^4 + 10^3
// x * x * x * (x * (6 * x - 15) + 10)
static inline XMVECTOR Fade(FXMVECTOR v)
{
    XMVECTOR vv = XMVectorScale(v, 6.f);
    vv = XMVectorSubtract(vv, V15);
    vv = XMVectorMultiply(v, vv);
    vv = XMVectorAdd(vv, V10);
    XMVECTOR v3 = XMVectorMultiply(v, XMVectorMultiply(v, v));
    vv = XMVectorMultiply(v3, vv);

    return vv;
}

constexpr int GridLength = 256;

constexpr std::array<int, 512> Permutations{
    151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,   225, 140, 36,  103,
    30,  69,  142, 8,   99,  37,  240, 21,  10,  23,  190, 6,   148, 247, 120, 234, 75,  0,   26,
    197, 62,  94,  252, 219, 203, 117, 35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174,
    20,  125, 136, 171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158, 231,
    83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,  245, 40,  244, 102, 143,
    54,  65,  25,  63,  161, 1,   216, 80,  73,  209, 76,  132, 187, 208, 89,  18,  169, 200, 196,
    135, 130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124,
    123, 5,   202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,  58,  17,
    182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,  154, 163, 70,  221, 153, 101,
    155, 167, 43,  172, 9,   129, 22,  39,  253, 19,  98,  108, 110, 79,  113, 224, 232, 178, 185,
    112, 104, 218, 246, 97,  228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,
    51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184, 84,  204, 176,
    115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,  222, 114, 67,  29,  24,  72,  243,
    141, 128, 195, 78,  66,  215, 61,  156, 180, // first 256
    151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,   225, 140, 36,  103,
    30,  69,  142, 8,   99,  37,  240, 21,  10,  23,  190, 6,   148, 247, 120, 234, 75,  0,   26,
    197, 62,  94,  252, 219, 203, 117, 35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174,
    20,  125, 136, 171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158, 231,
    83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,  245, 40,  244, 102, 143,
    54,  65,  25,  63,  161, 1,   216, 80,  73,  209, 76,  132, 187, 208, 89,  18,  169, 200, 196,
    135, 130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124,
    123, 5,   202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,  58,  17,
    182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,  154, 163, 70,  221, 153, 101,
    155, 167, 43,  172, 9,   129, 22,  39,  253, 19,  98,  108, 110, 79,  113, 224, 232, 178, 185,
    112, 104, 218, 246, 97,  228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,
    51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184, 84,  204, 176,
    115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,  222, 114, 67,  29,  24,  72,  243,
    141, 128, 195, 78,  66,  215, 61,  156, 180, // mirror of first 256
};

constexpr float PointW = 1.f;
constexpr float VectorW = 0.f;
constexpr float GradientPlus = 0.7071068f;
constexpr float GradientMinus = -0.7071068f;

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

const XMVECTOR VX = XMVectorSet(1, 0, 0, VectorW);
const XMVECTOR VY = XMVectorSet(0, 1, 0, VectorW);
const XMVECTOR VZ = XMVectorSet(0, 0, 1, VectorW);

Perlin::Perlin()
{
}

float Perlin::Noise(float x, float y, float z)
{
    // original point, but positive
    XMVECTOR pXYZ = XMVectorSet(x, y, z, PointW);
    pXYZ = XMVectorAbs(pXYZ);

    // round down to the nearest integer
    XMVECTOR pUVW = XMVectorFloor(pXYZ);

    // vector inside the cube, relative
    XMVECTOR vUVW = XMVectorSubtract(pXYZ, pUVW);

    // grid coordinates
    XMFLOAT3 grid;
    XMStoreFloat3(&grid, pXYZ);
    int gx = static_cast<int>(grid.x) % GridLength;
    int gy = static_cast<int>(grid.y) % GridLength;
    int gz = static_cast<int>(grid.z) % GridLength;

    // the corner points of the unit cube
    XMVECTOR p000 =
        XMVectorSet(static_cast<float>(gx), static_cast<float>(gy), static_cast<float>(gz), PointW);
    XMVECTOR p001 = XMVectorAdd(p000, VZ);
    XMVECTOR p010 = XMVectorAdd(p000, VY);
    XMVECTOR p011 = XMVectorAdd(XMVectorAdd(p000, VZ), VY);
    XMVECTOR p100 = XMVectorAdd(p000, VX);
    XMVECTOR p101 = XMVectorAdd(p100, VZ);
    XMVECTOR p110 = XMVectorAdd(p100, VY);
    XMVECTOR p111 = XMVectorAdd(XMVectorAdd(p100, VZ), VY);

    // the point inside the cube that corresponds to our original x, y, z
    XMVECTOR pInsideCube = XMVectorAdd(p000, vUVW);

    // vectors from each corner point to the point inside the cube
    // TODO: Normalize them?
    XMVECTOR v000 = XMVectorSubtract(pInsideCube, p000);
    XMVECTOR v001 = XMVectorSubtract(pInsideCube, p001);
    XMVECTOR v010 = XMVectorSubtract(pInsideCube, p010);
    XMVECTOR v011 = XMVectorSubtract(pInsideCube, p011);
    XMVECTOR v100 = XMVectorSubtract(pInsideCube, p100);
    XMVECTOR v101 = XMVectorSubtract(pInsideCube, p101);
    XMVECTOR v110 = XMVectorSubtract(pInsideCube, p110);
    XMVECTOR v111 = XMVectorSubtract(pInsideCube, p111);

    XMVECTOR g000 = GradientAt(gx, gy, gz);
    XMVECTOR g001 = GradientAt(gx, gy, gz + 1);
    XMVECTOR g010 = GradientAt(gx, gy + 1, gz);
    XMVECTOR g011 = GradientAt(gx, gy + 1, gz + 1);
    XMVECTOR g100 = GradientAt(gx + 1, gy, gz);
    XMVECTOR g101 = GradientAt(gx + 1, gy, gz + 1);
    XMVECTOR g110 = GradientAt(gx + 1, gy + 1, gz);
    XMVECTOR g111 = GradientAt(gx + 1, gy + 1, gz + 1);

    XMVECTOR d000 = XMVector3Dot(g000, v000);
    XMVECTOR d001 = XMVector3Dot(g001, v001);
    XMVECTOR d010 = XMVector3Dot(g010, v010);
    XMVECTOR d011 = XMVector3Dot(g011, v011);
    XMVECTOR d100 = XMVector3Dot(g100, v100);
    XMVECTOR d101 = XMVector3Dot(g101, v101);
    XMVECTOR d110 = XMVector3Dot(g110, v110);
    XMVECTOR d111 = XMVector3Dot(g111, v111);

    return 0.f;
}

int Perlin::Hash(int x, int y, int z)
{
    size_t indexXY = Permutations[x] + static_cast<size_t>(y);
    size_t indexXYZ = Permutations[indexXY + z];
    return Permutations[indexXYZ];
}

DirectX::XMVECTOR Perlin::GradientAt(int x, int y, int z)
{
    int h = Hash(x, y, z);
    int i = h & 0b1111;
    return Gradients[i];
}
