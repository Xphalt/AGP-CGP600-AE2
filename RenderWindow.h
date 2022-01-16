#pragma once
#include "Renderer.h"
#include <string>

class RenderWindow
{
public:
	RenderWindow(const RenderWindow&) = delete;
	static RenderWindow& GetInstance() { return s_instance; }

	HRESULT InitialiseWindow(std::string& m_windowName, HINSTANCE hInstance, int nCmdShow);

private:
	static RenderWindow s_instance;
	RenderWindow() {};
};