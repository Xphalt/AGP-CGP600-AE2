#include "RenderWindow.h"
#include <d3d11.h>

int APIENTRY wWinMain(_In_ HINSTANCE _hInstance,
	_In_opt_ HINSTANCE _hPrevInstance,
	_In_ LPWSTR _lpCmdLine,
	_In_ int _nCmdShow)
{
	RenderWindow* rw = new RenderWindow(_hInstance, "Title", "MyWindowClass", 800, 600);
	rw->InitialiseWindow();

	while (rw->ProccessMessages())
	{

	}

	return 0;
}