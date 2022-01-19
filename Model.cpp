#include "Model.h"

Model::~Model()
{
	if (m_pObject != nullptr) { delete m_pObject; m_pObject = nullptr; }
}

HRESULT Model::LoadObjModel(char* _filename, ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, ID3D11ShaderResourceView* _texture, ConstantBuffer<CB_VS_VertexShader>& _cb_vs_vertexShader)
{
	m_pDevice = _device;
	m_pDeviceContext = _deviceContext;
	m_pTexture = _texture;
	m_CB_VS_vertexShader = _cb_vs_vertexShader;

	m_pObject = new ObjFileModel(_filename, m_pDevice, m_pDeviceContext);

	if (m_pObject->filename == "FILE NOT LOADED") { return S_FALSE; }

	Vertex vertex[] =
	{
		Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f),
		Vertex(-0.5f,  0.5f, -0.5f, 0.0f, 0.0f),
		Vertex( 0.5f,  0.5f, -0.5f, 1.0f, 0.0f),
		Vertex( 0.5f, -0.5f, -0.5f, 1.0f, 1.0f),
		Vertex(-0.5f, -0.5f,  0.5f, 0.0f, 1.0f),
		Vertex(-0.5f,  0.5f,  0.5f, 0.0f, 0.0f),
		Vertex( 0.5f,  0.5f,  0.5f, 1.0f, 0.0f), 
		Vertex( 0.5f, -0.5f,  0.5f, 1.0f, 1.0f), 
	};

	HRESULT hr;
	hr = m_vertexBuffer.Initialise(m_pDevice, vertex, ARRAYSIZE(vertex));
	if (FAILED(hr)) { return hr; }

	DWORD indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		4, 7, 6,
		4, 6, 5,
		3, 2, 6,
		3, 6, 7,
		4, 5, 1,
		4, 1, 0,
		1, 5, 6,
		1, 6, 2,
		0, 3, 7,
		0, 7, 4,
	};
	
	hr = m_indicesBuffer.Initialise(m_pDevice, indices, ARRAYSIZE(indices));
	if (FAILED(hr)) { return hr; }

	

	return S_OK;
}

void Model::Draw(const XMMATRIX& _viewProjectionMatrix)
{
	m_CB_VS_vertexShader.data.WorldViewProjection = m_worldMatrix * _viewProjectionMatrix;
	m_CB_VS_vertexShader.data.WorldViewProjection = XMMatrixTranspose(m_CB_VS_vertexShader.data.WorldViewProjection);
	m_CB_VS_vertexShader.ApplyChanges();

	m_pDeviceContext->VSSetConstantBuffers(0, 1, m_CB_VS_vertexShader.GetBufferAddress());
	m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTexture);
	m_pDeviceContext->IASetIndexBuffer(m_indicesBuffer.GetBuffer(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetBufferAddress(), m_vertexBuffer.GetStrideAddress(), &offset);
	m_pDeviceContext->DrawIndexed(m_indicesBuffer.BufferSize(), 0, 0);
}

void Model::AddTexture(ID3D11ShaderResourceView* _texture)
{
	m_pTexture = _texture;
}

void Model::UpdateWorldMatrix()
{
	m_worldMatrix = XMMatrixIdentity();
}

#pragma region Setters
void Model::SetXPos(float _xPos)
{
	m_x = _xPos;
}

void Model::SetYPos(float _yPos)
{
	m_y = _yPos;
}

void Model::SetZPos(float _zPos)
{
	m_z = _zPos;
}

void Model::SetXAngle(float _xAngle)
{
	m_xAngle = _xAngle;
}

void Model::SetYAngle(float _yAngle)
{
	m_yAngle = _yAngle;
}

void Model::SetZAngle(float _zAngle)
{
	m_zAngle = _zAngle;
}

void Model::SetScale(float _scale)
{
	m_scale = _scale;
}
#pragma endregion

#pragma region Increments
void Model::IncXPos(float _xPos)
{
	m_x += _xPos;
}

void Model::IncYPos(float _yPos)
{
	m_y += _yPos;
}

void Model::IncZPos(float _zPos)
{
	m_z += _zPos;
}

void Model::IncXAngle(float _xAngle)
{
	m_xAngle += _xAngle;
}

void Model::IncYAngle(float _yAngle)
{
	m_yAngle += _yAngle;
}

void Model::IncZAngle(float _zAngle)
{
	m_zAngle += _zAngle;
}

void Model::IncScale(float _scale)
{
	m_scale += _scale;
}
#pragma endregion