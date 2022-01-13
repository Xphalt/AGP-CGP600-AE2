#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <D3DX11.h>
#include <windows.h>
#include <dxerr.h>
#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <DirectXMath.h>

class InitDirectX;
class RenderWindow;
class Renderer;
class GameObject;
class Model;
class Shader;
class Texture;
class Lighting;

using namespace DirectX;

class GraphicsComponent
{
public:
	GraphicsComponent(InitDirectX* _initDirectX, RenderWindow* _renderWindow);
	GraphicsComponent(Model* _model, Shader* _shader, Texture* _texture, Lighting* _lighting, Renderer* _renderer);
	~GraphicsComponent();

	void Update(GameObject& _gameObject);
	InitDirectX* GetInitDirectX() const { return m_pInitDirectX; }

private:
	InitDirectX* m_pInitDirectX{ nullptr };
	RenderWindow* m_pRenderWindow{ nullptr };
	Renderer* m_pRenderer{ nullptr };
	GameObject* m_pGameObject{ nullptr };
	Model* m_pModel{ nullptr };
	Shader* m_pShader{ nullptr };
	Texture* m_pTexture{ nullptr };
	Lighting* m_pLighting{ nullptr };
};