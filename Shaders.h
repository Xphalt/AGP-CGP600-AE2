#pragma once
#include "Renderer.h"
#include <string>
#include <d3dcompiler.h>

using namespace DirectX;

class Shaders
{
public:
	Shaders();
	~Shaders();

	void InitialiseVertexShader(ID3D11Device* _device, std::wstring _shaderPath, D3D11_INPUT_ELEMENT_DESC* _inputDesc, UINT _numElements);
	void InitialisePixelShader(ID3D11Device* _device, std::wstring _shaderPath, D3D11_INPUT_ELEMENT_DESC* _inputDesc, UINT _numElements);

	ID3D11VertexShader* GetVertexShader() const { return m_pVertexShader; }
	ID3D11PixelShader* GetPixelShader() const { return m_pPixelShader; }
	ID3D10Blob* GetVertexShaderBuffer() { return m_pVertexShaderBuffer; }
	ID3D10Blob* GetPixelShaderBuffer() { return m_pPixelShaderBuffer; }
	ID3D11InputLayout* GetInputLayout() const { return m_pInputLayout; }

private:
	ID3D11VertexShader* m_pVertexShader{ nullptr };
	ID3D11PixelShader* m_pPixelShader{ nullptr };
	ID3D10Blob* m_pVertexShaderBuffer{ nullptr };
	ID3D10Blob* m_pPixelShaderBuffer{ nullptr };
	ID3D11InputLayout* m_pInputLayout{ nullptr };
};