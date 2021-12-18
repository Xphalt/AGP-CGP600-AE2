//IMPORTANT: Includes must be in this order:
#include <d3d11.h>
#include <dxgi.h>
#include <windows.h>

int WINAPI WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrev, _In_ LPSTR lpCmd, _In_ int nCmdShow)
{
    D3D11CreateDeviceAndSwapChain(  NULL,
                                    D3D_DRIVER_TYPE_REFERENCE,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    D3D11_SDK_VERSION,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL  );

    return 0;
}