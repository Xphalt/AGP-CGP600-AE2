#pragma once
#include <d3d11.h>
#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <DirectXMath.h>
#include <string>

using namespace DirectX;

class RenderWindow
{
public:
	RenderWindow(LPCSTR _windowName, HWND _hWnd, HINSTANCE _hInstance, int _nCmdShow);

private:
	HRESULT InitialiseWindow(LPCSTR _windowName, HWND _hWnd, HINSTANCE hInstance, int nCmdShow);
};