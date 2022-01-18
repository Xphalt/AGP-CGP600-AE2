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
	float backgroundColour[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GetDeviceContext()->ClearRenderTargetView(GetRenderTargetView(), backgroundColour);
	m_pDeviceContext->ClearDepthStencilView(GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	GetDeviceContext()->IASetInputLayout(GetShaders()->GetInputLayout());
	GetDeviceContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetDeviceContext()->RSSetState(GetRasterState());
	GetDeviceContext()->OMSetDepthStencilState(GetDSS(), 0);
	GetDeviceContext()->PSSetSamplers(0, 1, &m_pSamplerState);

	GetDeviceContext()->VSSetShader(GetShaders()->GetVertexShader(), NULL, 0);
	GetDeviceContext()->PSSetShader(GetShaders()->GetPixelShader(), NULL, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	GetDeviceContext()->PSSetShaderResources(0, 1, &m_pTexture);
	GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	GetDeviceContext()->Draw(6, 0);
	GetSwapChain()->Present(1, NULL);
}