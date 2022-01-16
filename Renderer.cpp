#pragma once
#include "Renderer.h"

Renderer Renderer::s_instance;

void Renderer::RenderFrame(void)
{
	float backgroundColour[] = { 0.1f, 0.2f, 0.6f, 1.0f };
	GetDeviceContext()->ClearRenderTargetView(GetRenderTargetView(), backgroundColour);
	GetDeviceContext()->IASetInputLayout(GetShaders()->GetInputLayout());
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetDeviceContext()->VSSetShader(GetShaders()->GetVertexShader(), NULL, 0);
	GetDeviceContext()->PSSetShader(GetShaders()->GetPixelShader(), NULL, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	GetDeviceContext()->IASetVertexBuffers(0, 1, &GetVertexBuffer(), &stride, &offset);
	GetDeviceContext()->Draw(3, 0);
	GetSwapChain()->Present(0, 0);
}