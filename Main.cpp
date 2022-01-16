#include "RenderWindow.h"
#include "Game.h"

int WINAPI WinMain(_In_ HINSTANCE _hInstance, _In_opt_ HINSTANCE _hPrevInstance, _In_ LPSTR _lpCmdLine, _In_ int _nCmdShow)
{
	std::string m_windowName{ "Linden's DirectX Game" };

	Renderer::GetInstance();
	RenderWindow::GetInstance().InitialiseWindow(m_windowName, _hInstance, _nCmdShow);
	Game* game = new Game();

	while (true)
	{
		game->Update();
	}

	delete game;
	game = nullptr;

	return 0;
}