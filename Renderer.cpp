#pragma once
#include "Renderer.h"

Renderer Renderer::s_instance;

Renderer::Renderer()
{
	m_pShaders = new Shaders();
}

Renderer::~Renderer()
{
	if (m_pSamplerState != nullptr) { m_pSamplerState = nullptr; }
	if (m_pTexture != nullptr) { m_pTexture = nullptr; }
	if (m_pDSS != nullptr) { m_pDSS = nullptr; }
	if (m_pDSB != nullptr) { m_pDSB = nullptr; }
	if (m_pDSV != nullptr) { m_pDSV = nullptr; }
	if (m_pRasterState != nullptr) { m_pRasterState = nullptr; }
	if (m_pShaders != nullptr) { m_pShaders = nullptr; }
	if (m_pRenderTargetView != nullptr) { m_pRenderTargetView = nullptr; }
	if (m_pSwapChain != nullptr) { m_pSwapChain = nullptr; }
	if (m_pDeviceContext != nullptr) { m_pDeviceContext = nullptr; }
	if (m_pDevice != nullptr) { m_pDevice = nullptr; }
	if (m_hwnd != NULL) { m_hwnd = NULL;}
	if (m_pShaders != nullptr) { delete m_pShaders; m_pShaders = nullptr; }
}

void Renderer::RenderFrame(void)
{
	float backgroundColour[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GetDeviceContext()->ClearRenderTargetView(GetRenderTargetView(), backgroundColour);
	GetDeviceContext()->ClearDepthStencilView(GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	GetDeviceContext()->IASetInputLayout(GetShaders()->GetInputLayout());
	GetDeviceContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetDeviceContext()->RSSetState(GetRasterState());
	GetDeviceContext()->OMSetDepthStencilState(GetDSS(), 0);
	GetDeviceContext()->PSSetSamplers(0, 1, &m_pSamplerState);

	GetDeviceContext()->VSSetShader(GetShaders()->GetVertexShader(), NULL, 0);
	GetDeviceContext()->PSSetShader(GetShaders()->GetPixelShader(), NULL, 0);

	UINT offset = 0;

	GetDeviceContext()->PSSetShaderResources(0, 1, &m_pTexture);
	GetDeviceContext()->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetBufferAddress(), m_pVertexBuffer.GetStrideAddress(), &offset);
	GetDeviceContext()->IASetIndexBuffer(m_pIndicesBuffer.GetBuffer(), DXGI_FORMAT_R32_UINT, 0);

	GetDeviceContext()->DrawIndexed(m_pIndicesBuffer.BufferSize(), 0, 0);
	GetSwapChain()->Present(1, NULL);
}