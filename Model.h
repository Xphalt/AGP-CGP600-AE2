#pragma once

#include "objfilemodel.h"

class Model
{
private:
	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;

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

public:
	Model(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext);
	~Model();
	HRESULT LoadObjModel(char* _filename);
	void Draw(XMMATRIX* _view, XMMATRIX* _projection);
	void AddTexture(char* _filename);

	// Setters
	void SetXPos(float _xPos);
	void SetYPos(float _yPos);
	void SetZPos(float _zPos);
	void SetXAngle(float _xAngle);
	void SetYAngle(float _yAngle);
	void SetZAngle(float _zAngle);
	void SetScale(float _scale);

	// Getters
	const float GetXPos() { return m_x; }
	const float GetZPos() { return m_y; }
	const float GetYPos() { return m_z; }
	const float GetXAngle() { return m_xAngle; }
	const float GetYAngle() { return m_yAngle; }
	const float GetZAngle() { return m_zAngle; }
	const float GetScale() { return m_scale; }

	// Increments
	void IncXPos(float _xPos);
	void IncYPos(float _yPos);
	void IncZPos(float _zPos);
	void IncXAngle(float _xAngle);
	void IncYAngle(float _yAngle);
	void IncZAngle(float _zAngle);
	void IncScale(float _scale);
};