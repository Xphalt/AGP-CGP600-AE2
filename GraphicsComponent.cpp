#include "GraphicsComponent.h"
#include "objfilemodel.h"

GraphicsComponent::~GraphicsComponent()
{
	if (m_pModel != nullptr) { delete m_pModel; m_pModel = { nullptr }; }
}

void GraphicsComponent::Update(GameObject& _gameObject)
{

}

void GraphicsComponent::SetModel(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, char* _filename)
{
	m_pModel = new ObjFileModel(_filename, _device, _deviceContext);
}

void GraphicsComponent::SetTexture(ID3D11Device* _device, char* _filename)
{
	D3DX11CreateShaderResourceViewFromFile(_device, _filename, NULL, NULL, &m_pTexture, NULL);
}