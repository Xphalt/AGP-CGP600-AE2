#pragma once
#include "GraphicsComponent.h"

//struct CONSTANT_BUFFER0
//{
//	XMMATRIX WorldViewProjection;               // 64 bytes
//	XMVECTOR directional_light_vector;          // 16 bytes
//	XMVECTOR directional_light_colour;          // 16 bytes
//	XMVECTOR ambient_light_colour;              // 16 bytes
//
//	// TOTAL SIZE = 112 bytes
//};
//
//struct POS_COL_TEX_NORM_VERTEX
//{
//	XMFLOAT3 pos;
//	XMFLOAT4 col;
//	XMFLOAT2 texture0;
//	XMFLOAT3 normal;
//};

class InitDirectX
{
public:
	InitDirectX();
	~InitDirectX();

	HRESULT Initialise(HWND _hwnd);

private:
	D3D_DRIVER_TYPE m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;



	//ID3D11Buffer* m_pVertexBuffer;
	//ID3D11VertexShader* m_pVertexShader;
	//ID3D11PixelShader* m_pPixelShader;
	//ID3D11InputLayout* m_pInputLayout;
	//ID3D11Buffer* m_pConstantBuffer0;
	//CONSTANT_BUFFER0 m_cb0Values;
	//ID3D11DepthStencilView* m_pZBuffer;
	//ID3D11ShaderResourceView* m_pTexture0;
	//ID3D11SamplerState* m_pSampler0;
	//ID3D11BlendState* m_pAlphaBlendEnable;
	//ID3D11BlendState* m_pAlphaBlendDisable;
	//ID3D11RasterizerState* rastStateCullNone;
	//ID3D11RasterizerState* rastStateCullBack;
	//Model* m_model;

	//Camera* m_camera;
	//Text2D* m_2DText;
	//XMVECTOR m_directional_light_shines_from;
	//XMVECTOR m_directional_light_colour;
	//XMVECTOR m_ambient_light_colour;
};