#pragma once
#include "Shaders.h"

Shaders::Shaders()
{
}

Shaders::~Shaders()
{
	if (m_pPixelShader) { m_pPixelShader->Release(); m_pPixelShader = nullptr; }
	//if (m_pPixelShaderBuffer) { m_pPixelShaderBuffer->Release(); m_pPixelShaderBuffer = nullptr; }
	if (m_pInputLayout) { m_pInputLayout->Release(); m_pInputLayout = nullptr; }
	if (m_pVertexShader) { m_pVertexShader->Release(); m_pVertexShader = nullptr; }
	//if (m_pVertexShaderBuffer) { m_pVertexShaderBuffer->Release(); m_pVertexShaderBuffer = nullptr;	}
}

HRESULT Shaders::InitialiseVertexShader(std::wstring _shaderPath, D3D11_INPUT_ELEMENT_DESC* _inputDesc, UINT _numElements)
{
	ID3D10Blob* m_pVertexShaderBuffer;

	HRESULT hr = D3DReadFileToBlob(_shaderPath.c_str(), &m_pVertexShaderBuffer);

	if (FAILED(hr)) { return hr; }

	//SIZE_T bufferSize = m_pVertexShaderBuffer->GetBufferSize();
	//ID3D11InputLayout = (ID3D11InputLayout**)GetInputLayout();

	hr = Renderer::GetInstance().GetDevice()->CreateVertexShader(&m_pVertexShaderBuffer, m_pVertexShaderBuffer->GetBufferSize(), NULL, (ID3D11VertexShader**)GetVertexShader());
	hr = Renderer::GetInstance().GetDevice()->CreateInputLayout(_inputDesc, _numElements, m_pVertexShaderBuffer, m_pVertexShaderBuffer->GetBufferSize(), (ID3D11InputLayout**)GetInputLayout());

	return S_OK;
}

HRESULT Shaders::InitialisePixelShader(std::wstring _shaderPath, D3D11_INPUT_ELEMENT_DESC* _inputDesc, UINT _numElements)
{
	ID3D10Blob* m_pPixelShaderBuffer;

	HRESULT hr = D3DReadFileToBlob(_shaderPath.c_str(), &m_pPixelShaderBuffer);

	if (FAILED(hr)) { return hr; }

	hr = Renderer::GetInstance().GetDevice()->CreatePixelShader(&m_pPixelShaderBuffer, m_pPixelShaderBuffer->GetBufferSize(), NULL, (ID3D11PixelShader**)GetPixelShader());

	return S_OK;
}