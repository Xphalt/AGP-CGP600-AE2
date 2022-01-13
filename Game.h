#pragma once
#include "InitDirectX.h"
#include "RenderWindow.h"
#include "GraphicsComponent.h"

class InitDirectX;
class RenderWindow;
class GraphicsComponent;

class Game
{
public:
	Game();
	~Game();
	void Update();

private:
	InitDirectX* m_pInitDirectX{ nullptr };
	RenderWindow* m_pRenderWindow{ nullptr };
	GraphicsComponent* m_pGraphics{ nullptr };

	const LPCSTR m_windowName{ "Test" };
	HWND m_hwnd{ NULL };
	HINSTANCE m_hInstance{ NULL };
	int m_nCmdShow{ NULL };
};