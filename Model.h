#pragma once
#include "VertexBuffer.h"
#include "IndicesBuffer.h"
#include "ConstantBuffer.h"
#include "objfilemodel.h"

class GameObject;

class Model
{
public:
	~Model();

	HRESULT LoadObjModel(char* _filename, ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, ID3D11ShaderResourceView* _texture, ConstantBuffer<CB_VS_VertexShader>& _cb_vs_vertexShader);
	void Draw(const XMMATRIX& _viewProjectionMatrix);
	void AddTexture(ID3D11ShaderResourceView* _texture);

	// Getters
	const float GetXPos() const { return m_x; }
	const float GetZPos() const { return m_y; }
	const float GetYPos() const { return m_z; }
	const float GetXAngle() const { return m_xAngle; }
	const float GetYAngle() const{ return m_yAngle; }
	const float GetZAngle() const { return m_zAngle; }
	const float GetScale() const { return m_scale; }

	// Setters
	void SetXPos(float _xPos);
	void SetYPos(float _yPos);
	void SetZPos(float _zPos);
	void SetXAngle(float _xAngle);
	void SetYAngle(float _yAngle);
	void SetZAngle(float _zAngle);
	void SetScale(float _scale);

	// Increments
	void IncXPos(float _xPos);
	void IncYPos(float _yPos);
	void IncZPos(float _zPos);
	void IncXAngle(float _xAngle);
	void IncYAngle(float _yAngle);
	void IncZAngle(float _zAngle);
	void IncScale(float _scale);

private:
	void UpdateWorldMatrix();

	ID3D11Device* m_pDevice{ nullptr };
	ID3D11DeviceContext* m_pDeviceContext{ nullptr };

	ID3D11ShaderResourceView* m_pTexture{ nullptr };

	ConstantBuffer<CB_VS_VertexShader> m_CB_VS_vertexShader;
	VertexBuffer<Vertex> m_vertexBuffer;
	IndicesBuffer m_indicesBuffer;

	ObjFileModel* m_pObject;

	XMMATRIX m_worldMatrix = XMMatrixIdentity();

	float m_x, m_y, m_z;
	float m_xAngle, m_yAngle, m_zAngle;
	float m_scale;
};