#pragma once
#include "d3d11.h"

class Renderer;

template<typename T>
class VertexBuffer
{
public:
	VertexBuffer() {}

	ID3D11Buffer* const* GetBufferAddress() const { return &m_pBuffer; }
	UINT BufferSize() const { return m_bufferSize; }
	const UINT* GetStrideAddress() const { return &m_pStride; }

	HRESULT Initialise(ID3D11Device* _device, T* _data, UINT _numOfVertices)
	{
		m_bufferSize = _numOfVertices;
		m_pStride = (UINT)sizeof(T);

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.ByteWidth = sizeof(T) * _numOfVertices;

		HRESULT hr = _device->CreateBuffer(&vertexBufferDesc, NULL, &m_pBuffer);
		if (FAILED(hr)) { return hr; }

		D3D11_MAPPED_SUBRESOURCE ms;
		Renderer::GetInstance().GetDeviceContext()->Map(m_pBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);	// Lock the buffer to allow writing
		memcpy(ms.pData, _data, sizeof(_data[0]) * _numOfVertices);							// Copy the data
		Renderer::GetInstance().GetDeviceContext()->Unmap(m_pBuffer, NULL);

		return S_OK;
	}

private:
	VertexBuffer(const VertexBuffer<T>& rhs);

	ID3D11Buffer* m_pBuffer{ nullptr };
	UINT m_pStride{ NULL };
	UINT m_bufferSize = { 0 };
};