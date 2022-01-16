#include "RenderWindow.h"
#include "Game.h"
#include <d3d11.h>

int APIENTRY wWinMain(_In_ HINSTANCE _hInstance, _In_opt_ HINSTANCE _hPrevInstance,	_In_ LPWSTR _lpCmdLine,	_In_ int _nCmdShow)

{
	RenderWindow::GetInstance().Initialise(_hInstance, "Title", "MyWindowClass", 800, 600);
	Game* game = new Game();

	while (RenderWindow::GetInstance().ProccessMessages() == true)
	{
		game->Update();
	}

	return 0;
}