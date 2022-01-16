#pragma once
#include "Shaders.h"

Shaders::Shaders()
{
}

Shaders::~Shaders()
{
}

void Shaders::InitialiseVertexShader(ID3D11Device* _device, std::wstring _shaderPath, D3D11_INPUT_ELEMENT_DESC* _inputDesc, UINT _numElements)
{
	HRESULT hr = D3DReadFileToBlob(_shaderPath.c_str(), (ID3DBlob**)GetVertexShaderBuffer()); //TODO - May not work
	hr = _device->CreateVertexShader(m_pVertexShaderBuffer, GetVertexShaderBuffer()->GetBufferSize(), NULL, &m_pVertexShader);
	hr = _device->CreateInputLayout(_inputDesc, _numElements, GetVertexShaderBuffer(), GetVertexShaderBuffer()->GetBufferSize(), &m_pInputLayout);
}

void Shaders::InitialisePixelShader(ID3D11Device* _device, std::wstring _shaderPath, D3D11_INPUT_ELEMENT_DESC* _inputDesc, UINT _numElements)
{
	HRESULT hr = D3DReadFileToBlob(_shaderPath.c_str(), (ID3DBlob**)GetPixelShaderBuffer());
	hr = _device->CreatePixelShader(GetPixelShaderBuffer(), GetPixelShaderBuffer()->GetBufferSize(), NULL, &m_pPixelShader);
}