#pragma once
#include <d3d11.h>

class Model;

class GameObject
{
public:
	GameObject(float _xPos, float _yPos, float _zPos, const char* _filename, ID3D11Device* _device, ID3D11DeviceContext* _deviceContext);
	~GameObject();

	void Update();

private:
	Model* m_gameObject{ nullptr };
};