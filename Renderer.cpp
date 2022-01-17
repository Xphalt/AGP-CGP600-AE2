#pragma once
#include "Renderer.h"

Renderer Renderer::s_instance;

Renderer::Renderer()
{
	m_pShaders = new Shaders();
}

Renderer::~Renderer()
{

	if (m_pShaders != nullptr) { delete m_pShaders; m_pShaders = nullptr; }
}

void Renderer::RenderFrame(void)
{
	float backgroundColour[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GetDeviceContext()->ClearRenderTargetView(GetRenderTargetView(), backgroundColour);
	GetDeviceContext()->IASetInputLayout(GetShaders()->GetInputLayout());
	GetDeviceContext()->VSSetShader(GetShaders()->GetVertexShader(), NULL, 0);
	GetDeviceContext()->PSSetShader(GetShaders()->GetPixelShader(), NULL, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetDeviceContext()->Draw(3, 0);
	GetSwapChain()->Present(0, 0);
}