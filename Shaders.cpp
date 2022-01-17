#pragma once
#include "Shaders.h"

Shaders::Shaders()
{
}

Shaders::~Shaders()
{
	if (m_pPixelShader) { m_pPixelShader->Release(); m_pPixelShader = nullptr; }
	if (m_pPixelShaderBuffer) { m_pPixelShaderBuffer->Release(); m_pPixelShaderBuffer = nullptr; }
	if (m_pInputLayout) { m_pInputLayout->Release(); m_pInputLayout = nullptr; }
	if (m_pVertexShader) { m_pVertexShader->Release(); m_pVertexShader = nullptr; }
	if (m_pVertexShaderBuffer) { m_pVertexShaderBuffer->Release(); m_pVertexShaderBuffer = nullptr;	}
}

HRESULT Shaders::InitialiseVertexShader(std::wstring _shaderPath, D3D11_INPUT_ELEMENT_DESC* _inputDesc, UINT _numElements)
{
	HRESULT hr = D3DReadFileToBlob(_shaderPath.c_str(), &m_pVertexShaderBuffer);

	if (FAILED(hr)) { return hr; }

	hr = Renderer::GetInstance().GetDevice()->CreateVertexShader(m_pVertexShaderBuffer->GetBufferPointer(), m_pVertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader);
	
	if (FAILED(hr)) { return hr; }

	hr = Renderer::GetInstance().GetDevice()->CreateInputLayout(_inputDesc, _numElements, m_pVertexShaderBuffer->GetBufferPointer(), m_pVertexShaderBuffer->GetBufferSize(), &m_pInputLayout);

	if (FAILED(hr)) { return hr; }

	return S_OK;
}

HRESULT Shaders::InitialisePixelShader(std::wstring _shaderPath, D3D11_INPUT_ELEMENT_DESC* _inputDesc, UINT _numElements)
{
	HRESULT hr = D3DReadFileToBlob(_shaderPath.c_str(), &m_pPixelShaderBuffer);

	if (FAILED(hr)) { return hr; }

	hr = Renderer::GetInstance().GetDevice()->CreatePixelShader(m_pPixelShaderBuffer->GetBufferPointer(), m_pPixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader);

	if (FAILED(hr)) { return hr; }

	return S_OK;
}