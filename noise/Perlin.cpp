#include "Perlin.h"

using namespace DirectX;

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

Perlin::Perlin()
{
    InitGradients();
}

float Perlin::Noise(float x, float y, float z)
{
    float xAbs = std::abs(x);
    float yAbs = std::abs(y);
    float zAbs = std::abs(z);

    // grid coordinates
    int x0 = static_cast<int>(xAbs) % GridLength;
    int y0 = static_cast<int>(yAbs) % GridLength;
    int z0 = static_cast<int>(zAbs) % GridLength;

    // constant w for building vectors
    constexpr int iw1 = 1;
    constexpr float fw1 = 1.f;

    // vectors pointing to the corners of the unit cube
    XMVECTOR v000 = XMVectorSetInt(x0, y0, z0, iw1);
    XMVECTOR v001 = XMVectorSetInt(x0, y0, z0 + 1, iw1);
    XMVECTOR v010 = XMVectorSetInt(x0, y0 + 1, z0, iw1);
    XMVECTOR v011 = XMVectorSetInt(x0, y0 + 1, z0 + 1, iw1);
    XMVECTOR v100 = XMVectorSetInt(x0 + 1, y0, z0, iw1);
    XMVECTOR v101 = XMVectorSetInt(x0 + 1, y0, z0 + 1, iw1);
    XMVECTOR v110 = XMVectorSetInt(x0 + 1, y0 + 1, z0, iw1);
    XMVECTOR v111 = XMVectorSetInt(x0 + 1, y0 + 1, z0 + 1, iw1);

    // relative coordinates of our actual point
    float u = xAbs - x0;
    float v = yAbs - y0;
    float w = zAbs - z0;

    // smooth the relative coordinates
    float uSmooth = Fade(u);
    float vSmooth = Fade(v);
    float wSmooth = Fade(w);

    // vector to our point inside the unit cube
    XMVECTOR xyz = XMVectorAdd(XMVectorSet(u, v, w, fw1), v000);

    return 0.f;
}

void Perlin::InitGradients()
{
    constexpr std::array<float, 2> values{+1.f, -1.f};
    constexpr float w = 1.f;

    int index = 0;

    for (const auto v1 : values)
    {
        for (const auto v2 : values)
        {
            m_gradients[index++] = XMVectorSet(0, v1, v2, w);
            m_gradients[index++] = XMVectorSet(v1, 0, v2, w);
            m_gradients[index++] = XMVectorSet(v1, v2, 0, w);
        }
    }

    // repeat some gradients
    m_gradients[12] = XMVectorSet(1, 1, 0, 0);
    m_gradients[13] = XMVectorSet(-1, 1, 0, 0);
    m_gradients[14] = XMVectorSet(0, -1, 1, 0);
    m_gradients[15] = XMVectorSet(0, -1, -1, 0);

    // normalize
    for (int i = 0; i < m_gradients.size(); ++i)
    {
        m_gradients[i] = XMVector3Normalize(m_gradients[i]);
    }
}

DirectX::XMVECTOR Perlin::Gradient(int i)
{
    return m_gradients[i];
}

int Perlin::Hash(int x, int y, int z)
{
    size_t indexXY = Permutations[x] + static_cast<size_t>(y);
    size_t indexXYZ = Permutations[indexXY + z];
    return Permutations[indexXYZ];
}
