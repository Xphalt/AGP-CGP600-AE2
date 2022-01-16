#pragma once
#include "Shaders.h"

Shaders::Shaders()
{
}

Shaders::~Shaders()
{
}

void Shaders::InitialiseVertexShader(std::wstring _shaderPath, D3D11_INPUT_ELEMENT_DESC* _inputDesc, UINT _numElements)
{
	HRESULT hr = D3DReadFileToBlob(_shaderPath.c_str(), &GetVertexShaderBuffer());
	hr = Renderer::GetInstance().GetDevice()->CreateVertexShader(GetVertexShaderBuffer(), GetVertexShaderBuffer()->GetBufferSize(), NULL, &GetVertexShader());
	hr = Renderer::GetInstance().GetDevice()->CreateInputLayout(_inputDesc, _numElements, GetVertexShaderBuffer(), GetVertexShaderBuffer()->GetBufferSize(), &GetInputLayout());
}

void Shaders::InitialisePixelShader(std::wstring _shaderPath, D3D11_INPUT_ELEMENT_DESC* _inputDesc, UINT _numElements)
{
	HRESULT hr = D3DReadFileToBlob(_shaderPath.c_str(), &GetPixelShaderBuffer());
	hr = Renderer::GetInstance().GetDevice()->CreatePixelShader(GetPixelShaderBuffer(), GetPixelShaderBuffer()->GetBufferSize(), NULL, &GetPixelShader());
}