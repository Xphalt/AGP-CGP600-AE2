#pragma once
#include "RenderWindow.h"

RenderWindow RenderWindow::s_instance;

LRESULT CALLBACK WndProc(HWND _hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(_hwnd, &ps);
        EndPaint(_hwnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            DestroyWindow(_hwnd);
            return 0;
        }

    default:
        return DefWindowProc(_hwnd, message, wParam, lParam);
    }

    return 0;
}

HRESULT RenderWindow::InitialiseWindow(std::string& m_windowName, HINSTANCE _hInstance, int _nCmdShow)
{
    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = _hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);

    wcex.lpszClassName = m_windowName.c_str();

    if (!RegisterClassEx(&wcex)) { return E_FAIL; }

    RECT rc = { 0, 0, 640, 480 };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    m_hwnd = CreateWindow(m_windowName.c_str(), NULL, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, _hInstance, NULL);

    if (!m_hwnd) { return E_FAIL; }

    ShowWindow(m_hwnd, _nCmdShow);

    return S_OK;
}