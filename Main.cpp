#pragma once
#include "RenderWindow.h"
#include "InitDirectX.h"

Renderer& renderer{ Renderer::GetInstance() };
RenderWindow& renderWindow{ RenderWindow::GetInstance() };
InitDirectX& initDirectX{ InitDirectX::GetInstance() };

std::string m_windowName{ "Linden's DirectX Game" };

int WINAPI WinMain(_In_ HINSTANCE _hInstance, _In_opt_ HINSTANCE _hPrevInstance, _In_ LPSTR _lpCmdLine, _In_ int _nCmdShow)
{
    UNREFERENCED_PARAMETER(_hPrevInstance);
    UNREFERENCED_PARAMETER(_lpCmdLine);

    if (FAILED(renderWindow.InitialiseWindow(m_windowName, _hInstance, _nCmdShow)))
    {
        return 0;
    }

    initDirectX.Initialise();

    MSG msg = { 0 };

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        else
        {
            renderer.RenderFrame();
        }
    }    

    return (int)msg.wParam;
}