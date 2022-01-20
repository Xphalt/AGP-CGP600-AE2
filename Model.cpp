#include "Model.h"

struct CONSTANT_BUFFER0
{
	XMMATRIX WorldViewProjection;               // 64 bytes
	XMVECTOR directional_light_vector;          // 16 bytes
	XMVECTOR directional_light_colour;          // 16 bytes
	XMVECTOR ambient_light_colour;              // 16 bytes

	// TOTAL SIZE = 112 bytes
};

Model::Model(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, char* _filename)
	: m_pD3DDevice {_device}, m_pImmediateContext {_deviceContext}
{
	m_x, m_y, m_z = { 0.0f };
	m_xAngle, m_yAngle, m_zAngle = { 0.0f };
	m_scale = { 1.0f };

	m_pObject = new ObjFileModel(_filename, m_pD3DDevice, m_pImmediateContext);
}

Model::~Model()
{
	if (m_pSampler) { m_pSampler->Release(); }
	if (m_pTexture) { m_pTexture->Release(); }
	if (m_pConstantBuffer) { m_pConstantBuffer->Release(); }
	if (m_pInputLayout) { m_pInputLayout->Release(); }
	if (m_pPShader) { m_pPShader->Release(); }
	if (m_pVShader) { m_pVShader->Release(); }
	if (m_pObject) { delete m_pObject; }
}

HRESULT Model::InitObjModel()
{
	if (m_pObject->filename == "FILE NOT LOADED") { return S_FALSE; }

	HRESULT hr = S_OK;
	ID3DBlob* VS, * PS, * error;

	// Load and compile the pixel + vertex shaders - use vs_5_0 to target DX11 hardware only
	hr = D3DX11CompileFromFile("model_shaders.hlsl", 0, 0, "ModelVS", "vs_4_0", 0, 0, 0, &VS, &error, 0);

	// Check for shader compilation error
	if (error != 0)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();

		// Don't fail if error is just warning
		if (FAILED(hr)) { return hr; }
	}

	hr = D3DX11CompileFromFile("model_shaders.hlsl", 0, 0, "ModelPS", "ps_4_0", 0, 0, 0, &PS, &error, 0);

	// Check for shader compilation error
	if (error != 0)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();

		// Don't fail if error is just warning
		if (FAILED(hr)) { return hr; }
	}


	// Create and set the input layout object
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		// Be very careful setting the correct dxgi format and D3D version
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create shader objects
	hr = m_pD3DDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pVShader);

	if (FAILED(hr)) { return hr; }

	hr = m_pD3DDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);

	if (FAILED(hr)) { return hr; }

	hr = m_pD3DDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pPShader);

	if (FAILED(hr)) { return hr; }

	// Create constant buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));
	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	constant_buffer_desc.ByteWidth = sizeof(CONSTANT_BUFFER0);
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hr = m_pD3DDevice->CreateBuffer(&constant_buffer_desc, NULL, &m_pConstantBuffer);

	CalculateModelCentrePoint();
	CalculateBoundingSphereRadius();

	return S_OK;
}

void Model::Draw(XMMATRIX* _view, XMMATRIX* _projection)
{
	// Create transformation

	XMMATRIX world;
	world = XMMatrixIdentity();
	world *= XMMatrixScaling(m_scale, m_scale, m_scale);
	world *= XMMatrixRotationX(XMConvertToRadians(m_xAngle));
	world *= XMMatrixRotationY(XMConvertToRadians(m_yAngle));
	world *= XMMatrixRotationZ(XMConvertToRadians(m_zAngle));
	world *= XMMatrixTranslation(m_x, m_y, m_z);

	// Create constant buffer structure

	CONSTANT_BUFFER0 model_cb_values;
	model_cb_values.WorldViewProjection = (world) * (*_view) * (*_projection);

	// Set constant buffer to be the active one
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &model_cb_values, 0, 0);

	// Set vertex and pixel shaders to be the active ones
	m_pImmediateContext->VSSetShader(m_pVShader, 0, 0);
	m_pImmediateContext->PSSetShader(m_pPShader, 0, 0);

	// Set input layer to be the active one
	m_pImmediateContext->IASetInputLayout(m_pInputLayout);

	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSampler);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTexture);

	m_pObject->Draw();
}

void Model::AddTexture(char* _filename)
{
	D3DX11CreateShaderResourceViewFromFile(m_pD3DDevice, "assets/BoxTexture.bmp", NULL, NULL, &m_pTexture, NULL);

	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	m_pD3DDevice->CreateSamplerState(&sampler_desc, &m_pSampler);
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

XMVECTOR Model::GetBoundingSphereWorldSpacePosition()
{
	XMMATRIX world = XMMatrixIdentity();

	world *= XMMatrixScaling(m_scale, m_scale, m_scale);
	world *= XMMatrixRotationX(XMConvertToRadians(m_xAngle));
	world *= XMMatrixRotationY(XMConvertToRadians(m_yAngle));
	world *= XMMatrixRotationZ(XMConvertToRadians(m_zAngle));
	world *= XMMatrixTranslation(m_boundingSphereCentreX, m_boundingSphereCentreY, m_boundingSphereCentreZ);

	XMVECTOR offset;
	offset = XMVectorSet(m_x, m_y, m_z, 0.0);
	offset = XMVector3Transform(offset, world);
	return offset;
}

float Model::GetBoundingSphereRadius()
{
	return (m_boundingSphereRadius * m_scale);
}

void Model::CalculateModelCentrePoint()
{
	float minx = m_pObject->vertices[0].Pos.x;
	float miny = m_pObject->vertices[0].Pos.y;
	float minz = m_pObject->vertices[0].Pos.z;
	float maxx = m_pObject->vertices[0].Pos.x;
	float maxy = m_pObject->vertices[0].Pos.y;
	float maxz = m_pObject->vertices[0].Pos.z;

	for (int i = 1; i < m_pObject->numverts; i++)
	{
		if (m_pObject->vertices[i].Pos.x < minx)
		{
			minx = m_pObject->vertices[i].Pos.x;
		}
		else if (m_pObject->vertices[i].Pos.x > maxx)
		{
			maxx = m_pObject->vertices[i].Pos.x;
		}
		if (m_pObject->vertices[i].Pos.y < miny)
		{
			miny = m_pObject->vertices[i].Pos.y;
		}
		else if (m_pObject->vertices[i].Pos.y > maxy)
		{
			maxy = m_pObject->vertices[i].Pos.y;
		}
		if (m_pObject->vertices[i].Pos.z < minz)
		{
			minz = m_pObject->vertices[i].Pos.z;
		}
		else if (m_pObject->vertices[i].Pos.z > maxz)
		{
			maxz = m_pObject->vertices[i].Pos.z;
		}
	}

	m_boundingSphereCentreX = (minx + maxx) / 2;
	m_boundingSphereCentreY = (miny + maxy) / 2;
	m_boundingSphereCentreZ = (minz + maxz) / 2;

}

void Model::CalculateBoundingSphereRadius()
{
	float maxDist = 0, checkDist;
	for (int i = 0; i < m_pObject->numverts; i++)
	{
		checkDist = sqrt(powf((m_boundingSphereCentreX - m_pObject->vertices[i].Pos.x), 2) + powf((m_boundingSphereCentreY - m_pObject->vertices[i].Pos.y), 2) + powf((m_boundingSphereCentreZ - m_pObject->vertices[i].Pos.z), 2));
		if (checkDist >= maxDist)	maxDist = checkDist;
	}

	m_boundingSphereRadius = maxDist;
}

bool Model::CheckCollision(Model* model)
{
	if (model == this)	return false;

	float x1 = XMVectorGetX(model->GetBoundingSphereWorldSpacePosition());
	float y1 = XMVectorGetY(model->GetBoundingSphereWorldSpacePosition());
	float z1 = XMVectorGetZ(model->GetBoundingSphereWorldSpacePosition());
	float x2 = XMVectorGetX(this->GetBoundingSphereWorldSpacePosition());
	float y2 = XMVectorGetY(this->GetBoundingSphereWorldSpacePosition());
	float z2 = XMVectorGetZ(this->GetBoundingSphereWorldSpacePosition());

	float distance_squared = (powf((x1 - x2), 2.0f) + powf((y1 - y2), 2.0f) + powf((z1 - z2), 2.0f));
	float radii_squared = powf((model->GetBoundingSphereRadius()) + (this->GetBoundingSphereRadius()), 2.0f);

	if (distance_squared < radii_squared)
	{
		return true;
	}
	else
	{
		return false;
	}
}