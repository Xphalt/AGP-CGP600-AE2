#pragma once
#include "Renderer.h"
#include "ConstantBufferTypes.h"

class Renderer;

template<typename T>
class ConstantBuffer
{
public:
	ConstantBuffer() {}

	T data;

	ID3D11Buffer* GetBuffer() const { return m_pBuffer; }
	ID3D11Buffer* const* GetBufferAddress() const { return &m_pBuffer; }

	HRESULT Initialize(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext)
	{
		m_pDeviceContext = _deviceContext;

		D3D11_BUFFER_DESC constantBufferdesc;
		constantBufferdesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantBufferdesc.MiscFlags = 0;
		constantBufferdesc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		constantBufferdesc.StructureByteStride = 0;

		HRESULT hr = _device->CreateBuffer(&constantBufferdesc, 0, &m_pBuffer);

		if (FAILED(hr)) { return hr; }

		return S_OK;
	}

	HRESULT ApplyChanges()
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = m_pDeviceContext->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		if (FAILED(hr)) { return hr; }

		CopyMemory(mappedResource.pData, &data, sizeof(T));
		m_pDeviceContext->Unmap(m_pBuffer, 0);

		return S_OK;
	}

private:
	ConstantBuffer(const ConstantBuffer<T>& rhs);

	ID3D11Buffer* m_pBuffer{ nullptr };
	ID3D11DeviceContext* m_pDeviceContext{ nullptr };
};