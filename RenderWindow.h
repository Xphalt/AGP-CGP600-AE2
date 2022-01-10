#pragma once
#include "ErrorLogger.h"

class RenderWindow
{
public:
	RenderWindow(HINSTANCE _hInstance, std::string _windowTitle, std::string _windowClass, int _width, int _height);;
	~RenderWindow();
	bool InitialiseWindow();
	bool ProccessMessages();

private:
	void RegisterWindowClass();
	HWND m_handle = { NULL };
	HINSTANCE m_hInstance = { NULL };
	std::string m_windowTitle = { "" };
	std::wstring m_windowTitleWide = { L"" };
	std::string m_windowClass = { "" };
	std::wstring m_windowClassWide = { L"" };
	int m_width = { 0 };
	int m_height = { 0 };
};