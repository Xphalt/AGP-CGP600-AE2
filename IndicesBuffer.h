#pragma once
#include <d3d11.h>

class IndicesBuffer
{
public:
	IndicesBuffer() {};

	ID3D11Buffer* GetBuffer() const { return m_pBuffer; }
	UINT BufferSize() const { return m_bufferSize; }

	HRESULT Initialise(ID3D11Device* _device, DWORD* _data, UINT _numOfIndices)
	{
		m_bufferSize = _numOfIndices;

		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(DWORD) * m_bufferSize;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA indexBufferData{ NULL };
		indexBufferData.pSysMem = _data;

		HRESULT hr = _device->CreateBuffer(&indexBufferDesc, &indexBufferData, &m_pBuffer);

		if (FAILED(hr)) { return hr; }

		return S_OK;
	}

private:
	IndicesBuffer(const IndicesBuffer& rhs);

	ID3D11Buffer* m_pBuffer{ nullptr };
	UINT m_bufferSize = { 0 };
};