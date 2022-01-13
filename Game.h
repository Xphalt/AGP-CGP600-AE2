#pragma once
#include <windows.h>

class InitDirectX;
class RenderWindow;

class Game
{
public:
	Game();
	~Game();
	void Update();

private:
	InitDirectX* m_pInitDirectX{ nullptr };
	RenderWindow* m_pRenderWindow{ nullptr };

	const LPCSTR m_windowName{ "Test" };
	HWND m_hwnd{ NULL };
	HINSTANCE m_hInstance{ NULL };
	int m_nCmdShow{ NULL };
};