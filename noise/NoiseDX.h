#pragma once

#include "stdafx.h"

namespace DXM
{

DirectX::XMVECTOR Fade(DirectX::FXMVECTOR t);

// Original implementation (improved noise)
float Grad0(int hash, float x, float y, float z);

// Will use either the original implementation, or the pure vector one
float Grad0(int hash, DirectX::FXMVECTOR v);

// Pure vector gradients
float Grad(int hash, DirectX::FXMVECTOR v);

// Pure vector gradients, with original interface
float Grad(int hash, float x, float y, float z);

float Noise(float x, float y, float z);

}; // namespace DXM
