#include "RenderWindow.h"

RenderWindow::RenderWindow(HINSTANCE _hInstance, std::string _windowTitle, std::string _windowClass, int _width, int _height)
	: m_hInstance {_hInstance}, m_windowTitle {_windowTitle}, m_windowClass {_windowClass}, m_width {_width}, m_height {_height}
{
	m_windowTitleWide = StringConverter::StringToWide(m_windowTitle);
	m_windowClassWide = StringConverter::StringToWide(m_windowClass);

	InitialiseWindow();
}

RenderWindow::~RenderWindow()
{
	if (m_handle != NULL)
	{
		UnregisterClass(m_windowClassWide.c_str(), m_hInstance);
		DestroyWindow(m_handle);
	}
}

bool RenderWindow::InitialiseWindow()
{
	RegisterWindowClass();

	m_handle = CreateWindowEx(
		0,
		m_windowClassWide.c_str(),
		m_windowTitleWide.c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		0,
		0,
		m_width,
		m_height,
		NULL,
		NULL,
		m_hInstance,
		nullptr);

	if (m_handle == NULL)
	{
		ErrorLogger::LogError(GetLastError(), "CreateWindowEx failed for window: " + m_windowTitle);

		return false;
	}

	ShowWindow(m_handle, SW_SHOW);
	SetForegroundWindow(m_handle);
	SetFocus(m_handle);

	return true;
}

bool RenderWindow::ProccessMessages()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	if (PeekMessage(&msg,
		m_handle,
		0,
		0,
		PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_NULL)
	{
		if (!IsWindow(m_handle))
		{
			m_handle = NULL;
			UnregisterClass(m_windowClassWide.c_str(), m_hInstance);

			return false;
		}
	}

	return true;
}

void RenderWindow::RegisterWindowClass()
{
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = DefWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_windowClassWide.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);
}