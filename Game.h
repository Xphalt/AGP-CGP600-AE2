#pragma once
#include "Renderer.h"
#include "InitDirectX.h"
#include "RenderWindow.h"

class Renderer;
class InitDirectX;

class Game
{
public:
	Game();
	~Game();

	void Update();

private:
	Renderer& m_rRenderer{ Renderer::GetInstance() };
	InitDirectX& m_rInitDirectX{ InitDirectX::GetInstance() };

	HWND m_hwnd{ NULL };
};