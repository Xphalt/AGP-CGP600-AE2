#pragma once
#include "Vertex.h"
#include "ConstantBuffer.h"
#include "objfilemodel.h"

class Model
{
public:
	~Model();

	HRESULT LoadObjModel(char* _filename, ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, ID3D11ShaderResourceView* _texture);
	void Draw(XMMATRIX* _view, XMMATRIX* _projection);
	void AddTexture(ID3D11ShaderResourceView* _texture);

private:
	XMMATRIX m_worldMatrix = XMMatrixIdentity();

	ObjFileModel* m_pObject;
	ID3D11Device* m_pDevice{ nullptr };
	ID3D11DeviceContext* m_pDeviceContext{ nullptr };

	ID3D11ShaderResourceView* m_pTexture{ nullptr };

	ConstantBuffer<CB_VS_VertexShader> m_CB_VS_vertexShader;

	float m_x, m_y, m_z;
	float m_xAngle, m_yAngle, m_zAngle;
	float m_scale;
};