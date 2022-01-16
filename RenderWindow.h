#pragma once
#include "Renderer.h"

class RenderWindow
{
public:
	RenderWindow(const RenderWindow&) = delete;
	static RenderWindow& GetInstance() { return s_instance; }

	void Initialise(HINSTANCE _hInstance, std::string _windowTitle, std::string _windowClass, int _width, int _height);
	bool InitialiseWindow();
	bool ProccessMessages();

private:
	RenderWindow() {}
	void RegisterWindowClass();

	static RenderWindow s_instance;
	HWND m_handle = { NULL };
	HINSTANCE m_hInstance = { NULL };
	std::string m_windowTitle = { "" };
	std::string m_windowClass = { "" };
	int m_width = { 0 };
	int m_height = { 0 };
};