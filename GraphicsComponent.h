#pragma once
#include <d3d11.h>

class GameObject;
class ObjFileModel;

class GraphicsComponent
{
public:
	~GraphicsComponent();

	void Update(GameObject& _gameObject);
	void SetModel(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, char* _filename);
	void SetTexture(ID3D11Device* _device, char* _filename);

private:
	ObjFileModel* m_pModel{ nullptr };
	ID3D11ShaderResourceView* m_pTexture{ nullptr };
};