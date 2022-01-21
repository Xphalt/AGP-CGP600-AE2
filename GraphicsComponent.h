#pragma once
#include <d3d11.h>
#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <DirectXMath.h>
class GameObject;
class ObjFileModel;

using namespace DirectX;

class GraphicsComponent
{
public:
	~GraphicsComponent();

	void Update(GameObject& _gameObject, ID3D11DeviceContext* _deviceContext, XMMATRIX* _view, XMMATRIX* _projection);
	HRESULT SetModel(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, char* _filename);
	void SetTexture(ID3D11Device* _device, char* _filename);
	HRESULT InitVertexShader(ID3D11Device* _device);
	HRESULT InitPixelShader(ID3D11Device* _device);
	HRESULT InitInputLayer(ID3D11Device* _device);
	HRESULT InitConstantBuffer(ID3D11Device* _device);

private:
	HRESULT m_hr{ S_OK };
	ID3DBlob* VS, * PS, * error;
	ObjFileModel* m_pObject;
	ID3D11VertexShader* m_pVShader;
	ID3D11PixelShader* m_pPShader;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11ShaderResourceView* m_pTexture;
	ID3D11SamplerState* m_pSampler;

	float m_x, m_y, m_z;
	float m_xAngle, m_yAngle, m_zAngle;
	float m_scale;
};