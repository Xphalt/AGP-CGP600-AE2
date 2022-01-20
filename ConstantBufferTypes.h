#pragma once
#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <DirectXMath.h>

struct CB_VS_VertexShader
{
	DirectX::XMMATRIX WorldViewProjection;
};