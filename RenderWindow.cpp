#pragma once
#include "RenderWindow.h"

RenderWindow::RenderWindow(LPCSTR _windowName, HWND _hWnd, HINSTANCE _hInstance, int _nCmdShow)
{
    InitialiseWindow(_windowName, _hWnd, _hInstance, _nCmdShow);
}

HRESULT RenderWindow::InitialiseWindow(LPCSTR _windowName, HWND _hWnd, HINSTANCE _hInstance, int _nCmdShow)
{
    // Register class
    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = _hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);

    // Needed for non-3D apps
    //wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    wcex.lpszClassName = _windowName;

    if (!RegisterClassEx(&wcex)) { return E_FAIL; }

    // Create window
    RECT rc = { 0, 0, 640, 480 };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    _hWnd = CreateWindow(_windowName, NULL, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, _hInstance, NULL);

    if (!_hWnd) { return E_FAIL; }

    ShowWindow(_hWnd, _nCmdShow);

    return S_OK;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            DestroyWindow(hWnd);
            return 0;
        }

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}