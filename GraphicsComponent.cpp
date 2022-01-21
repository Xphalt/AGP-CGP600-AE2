#include "GraphicsComponent.h"
#include "objfilemodel.h"

struct CONSTANT_BUFFER0
{
	XMMATRIX WorldViewProjection;               // 64 bytes
	XMVECTOR directional_light_vector;          // 16 bytes
	XMVECTOR directional_light_colour;          // 16 bytes
	XMVECTOR ambient_light_colour;              // 16 bytes

	// TOTAL SIZE = 112 bytes
};

GraphicsComponent::~GraphicsComponent()
{
	if (m_pObject != nullptr) { delete m_pObject; m_pObject = { nullptr }; }
}

void GraphicsComponent::Update(GameObject& _gameObject, ID3D11DeviceContext* _deviceContext, XMMATRIX* _view, XMMATRIX* _projection)
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
	_deviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	_deviceContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &model_cb_values, 0, 0);

	// Set vertex and pixel shaders to be the active ones
	_deviceContext->VSSetShader(m_pVShader, 0, 0);
	_deviceContext->PSSetShader(m_pPShader, 0, 0);

	// Set input layer to be the active one
	_deviceContext->IASetInputLayout(m_pInputLayout);

	_deviceContext->PSSetSamplers(0, 1, &m_pSampler);
	_deviceContext->PSSetShaderResources(0, 1, &m_pTexture);

	m_pObject->Draw();

}

HRESULT GraphicsComponent::SetModel(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, char* _filename)
{
	m_pObject = new ObjFileModel(_filename, _device, _deviceContext);
	if (m_pObject->filename == "FILE NOT LOADED") { return S_FALSE; }
}

void GraphicsComponent::SetTexture(ID3D11Device* _device, char* _filename)
{
	D3DX11CreateShaderResourceViewFromFile(_device, _filename, NULL, NULL, &m_pTexture, NULL);
}

HRESULT GraphicsComponent::InitVertexShader(ID3D11Device* _device)
{
	m_hr = D3DX11CompileFromFile("model_shaders.hlsl", 0, 0, "ModelVS", "vs_4_0", 0, 0, 0, &VS, &error, 0);

	if (error != 0)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();

		if (FAILED(m_hr)) { return m_hr; }
	}

	return m_hr;

	m_hr = _device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pVShader);

	if (FAILED(m_hr)) { return m_hr; }

	return m_hr;
}

HRESULT GraphicsComponent::InitPixelShader(ID3D11Device* _device)
{
	m_hr = D3DX11CompileFromFile("model_shaders.hlsl", 0, 0, "ModelPS", "ps_4_0", 0, 0, 0, &PS, &error, 0);

	if (error != 0)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();

		if (FAILED(m_hr)) { return m_hr; }
	}

	return m_hr;
}

HRESULT GraphicsComponent::InitInputLayer(ID3D11Device* _device)
{
	// Create and set the input layout object
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		// Be very careful setting the correct dxgi format and D3D version
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	m_hr = _device->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);

	if (FAILED(m_hr)) { return m_hr; }

	return m_hr;
}



HRESULT GraphicsComponent::InitConstantBuffer(ID3D11Device* _device)
{
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));
	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	constant_buffer_desc.ByteWidth = sizeof(CONSTANT_BUFFER0);
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	m_hr = _device->CreateBuffer(&constant_buffer_desc, NULL, &m_pConstantBuffer);
	if (FAILED(m_hr)) { return m_hr; }

	return m_hr;
}