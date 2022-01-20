#pragma once
#include "Renderer.h"
#include "Model.h"

Renderer Renderer::s_instance;

Renderer::Renderer()
{
	m_pShaders = new Shaders();
	m_pModel = new Model();
	m_pCamera = new Camera(0, 0, 0, 0);
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
	if (m_pCamera != nullptr) { m_pCamera = nullptr; }
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
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetDeviceContext()->RSSetState(GetRasterState());
	GetDeviceContext()->OMSetDepthStencilState(GetDSS(), 0);

	UINT offset = 0;

	XMMATRIX view, projection;

	view = m_pCamera->GetViewMatrix();

	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0), 640.0 / 480.0, 1.0, 100.0);

	m_pModel->Draw(&view, &projection);
	GetSwapChain()->Present(1, NULL);
}