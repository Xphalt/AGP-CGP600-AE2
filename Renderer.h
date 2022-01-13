#pragma once

class GameObject;
class Model;
class Camera;
class Text2D;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Update(GameObject& _gameObject);
	void RenderFrame(void);
private:
	D3D_DRIVER_TYPE m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device* m_pD3DDevice = nullptr;
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pBackBufferRTView = nullptr;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pConstantBuffer0;
	CONSTANT_BUFFER0 m_cb0Values;
	ID3D11DepthStencilView* m_pZBuffer;
	ID3D11ShaderResourceView* m_pTexture0;
	ID3D11SamplerState* m_pSampler0;
	ID3D11BlendState* m_pAlphaBlendEnable;
	ID3D11BlendState* m_pAlphaBlendDisable;
	ID3D11RasterizerState* rastStateCullNone;
	ID3D11RasterizerState* rastStateCullBack;
	Model* m_model{ nullptr };
	Camera* m_camera{ nullptr };
	Text2D* m_2DText{ nullptr };
};