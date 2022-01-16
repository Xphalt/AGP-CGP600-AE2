#pragma once
#include "Renderer.h"
#include <string>
#include <d3dcompiler.h>

class Shaders
{
public:
	Shaders();
	~Shaders();

	HRESULT InitialiseVertexShader(std::wstring _shaderPath, D3D11_INPUT_ELEMENT_DESC* _inputDesc, UINT _numElements);
	HRESULT InitialisePixelShader(std::wstring _shaderPath, D3D11_INPUT_ELEMENT_DESC* _inputDesc, UINT _numElements);

	ID3D11VertexShader* GetVertexShader() { return m_pVertexShader; }
	ID3D11PixelShader* GetPixelShader() { return m_pPixelShader; }
	//ID3D10Blob*& GetVertexShaderBuffer() { return m_pVertexShaderBuffer; }
	//ID3D10Blob*& GetPixelShaderBuffer() { return m_pPixelShaderBuffer; }
	ID3D11InputLayout* GetInputLayout() { return m_pInputLayout; }

private:
	ID3D11VertexShader* m_pVertexShader{ nullptr };
	ID3D11PixelShader* m_pPixelShader{ nullptr };
	ID3D11InputLayout* m_pInputLayout{ nullptr };
};