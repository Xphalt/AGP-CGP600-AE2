#pragma once
#include "d3d11.h"

template<class T>
class VertexBuffer
{
public:
	VertexBuffer() {}

	ID3D11Buffer* GetBuffer() const { return m_pBuffer; }
	ID3D11Buffer* const* GetBufferAddress() const { return &m_pBuffer; }
	UINT BufferSize() const { return m_bufferSize; }
	const UINT* GetStrideAddress() const { return &m_pStride; }

	HRESULT Initialise(ID3D11Device* _device, T* _data, UINT _numOfVertices)
	{
		m_bufferSize = _numOfVertices;
		m_pStride = (UINT)sizeof(T);

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(T) * m_bufferSize;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
		vertexBufferData.pSysMem = _data;

		HRESULT hr = _device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_pBuffer);

		if (FAILED(hr)) { return hr; }

		return S_OK;
	}

private:
	VertexBuffer(const VertexBuffer<T>& rhs);

	ID3D11Buffer* m_pBuffer{ nullptr };
	UINT m_pStride{ NULL };
	UINT m_bufferSize = { 0 };
};