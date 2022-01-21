#include <d3d11.h>
#include <dxgi.h>
#include <D3DX11.h>
#include <windows.h>
#include <dxerr.h>
#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <DirectXMath.h>
#include "Camera.h"
#include "text2D.h"
#include "Model.h"
#include <dinput.h>

#include "GameObject.h"
#include "GraphicsComponent.h"

#pragma region MyRegion
/****************************************************************************
 *
 *      DirectInput keyboard scan codes
 *
 ****************************************************************************/
#define DIK_ESCAPE          0x01
#define DIK_1               0x02
#define DIK_2               0x03
#define DIK_3               0x04
#define DIK_4               0x05
#define DIK_5               0x06
#define DIK_6               0x07
#define DIK_7               0x08
#define DIK_8               0x09
#define DIK_9               0x0A
#define DIK_0               0x0B
#define DIK_MINUS           0x0C    /* - on main keyboard */
#define DIK_EQUALS          0x0D
#define DIK_BACK            0x0E    /* backspace */
#define DIK_TAB             0x0F
#define DIK_Q               0x10
#define DIK_W               0x11
#define DIK_E               0x12
#define DIK_R               0x13
#define DIK_T               0x14
#define DIK_Y               0x15
#define DIK_U               0x16
#define DIK_I               0x17
#define DIK_O               0x18
#define DIK_P               0x19
#define DIK_LBRACKET        0x1A
#define DIK_RBRACKET        0x1B
#define DIK_RETURN          0x1C    /* Enter on main keyboard */
#define DIK_LCONTROL        0x1D
#define DIK_A               0x1E
#define DIK_S               0x1F
#define DIK_D               0x20
#define DIK_F               0x21
#define DIK_G               0x22
#define DIK_H               0x23
#define DIK_J               0x24
#define DIK_K               0x25
#define DIK_L               0x26
#define DIK_SEMICOLON       0x27
#define DIK_APOSTROPHE      0x28
#define DIK_GRAVE           0x29    /* accent grave */
#define DIK_LSHIFT          0x2A
#define DIK_BACKSLASH       0x2B
#define DIK_Z               0x2C
#define DIK_X               0x2D
#define DIK_C               0x2E
#define DIK_V               0x2F
#define DIK_B               0x30
#define DIK_N               0x31
#define DIK_M               0x32
#define DIK_COMMA           0x33
#define DIK_PERIOD          0x34    /* . on main keyboard */
#define DIK_SLASH           0x35    /* / on main keyboard */
#define DIK_RSHIFT          0x36
#define DIK_MULTIPLY        0x37    /* * on numeric keypad */
#define DIK_LMENU           0x38    /* left Alt */
#define DIK_SPACE           0x39
#define DIK_CAPITAL         0x3A
#define DIK_F1              0x3B
#define DIK_F2              0x3C
#define DIK_F3              0x3D
#define DIK_F4              0x3E
#define DIK_F5              0x3F
#define DIK_F6              0x40
#define DIK_F7              0x41
#define DIK_F8              0x42
#define DIK_F9              0x43
#define DIK_F10             0x44
#define DIK_NUMLOCK         0x45
#define DIK_SCROLL          0x46    /* Scroll Lock */
#define DIK_NUMPAD7         0x47
#define DIK_NUMPAD8         0x48
#define DIK_NUMPAD9         0x49
#define DIK_SUBTRACT        0x4A    /* - on numeric keypad */
#define DIK_NUMPAD4         0x4B
#define DIK_NUMPAD5         0x4C
#define DIK_NUMPAD6         0x4D
#define DIK_ADD             0x4E    /* + on numeric keypad */
#define DIK_NUMPAD1         0x4F
#define DIK_NUMPAD2         0x50
#define DIK_NUMPAD3         0x51
#define DIK_NUMPAD0         0x52
#define DIK_DECIMAL         0x53    /* . on numeric keypad */
#define DIK_OEM_102         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
#define DIK_F11             0x57
#define DIK_F12             0x58
#define DIK_F13             0x64    /*                     (NEC PC98) */
#define DIK_F14             0x65    /*                     (NEC PC98) */
#define DIK_F15             0x66    /*                     (NEC PC98) */
#define DIK_KANA            0x70    /* (Japanese keyboard)            */
#define DIK_ABNT_C1         0x73    /* /? on Brazilian keyboard */
#define DIK_CONVERT         0x79    /* (Japanese keyboard)            */
#define DIK_NOCONVERT       0x7B    /* (Japanese keyboard)            */
#define DIK_YEN             0x7D    /* (Japanese keyboard)            */
#define DIK_ABNT_C2         0x7E    /* Numpad . on Brazilian keyboard */
#define DIK_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
#define DIK_PREVTRACK       0x90    /* Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) */
#define DIK_AT              0x91    /*                     (NEC PC98) */
#define DIK_COLON           0x92    /*                     (NEC PC98) */
#define DIK_UNDERLINE       0x93    /*                     (NEC PC98) */
#define DIK_KANJI           0x94    /* (Japanese keyboard)            */
#define DIK_STOP            0x95    /*                     (NEC PC98) */
#define DIK_AX              0x96    /*                     (Japan AX) */
#define DIK_UNLABELED       0x97    /*                        (J3100) */
#define DIK_NEXTTRACK       0x99    /* Next Track */
#define DIK_NUMPADENTER     0x9C    /* Enter on numeric keypad */
#define DIK_RCONTROL        0x9D
#define DIK_MUTE            0xA0    /* Mute */
#define DIK_CALCULATOR      0xA1    /* Calculator */
#define DIK_PLAYPAUSE       0xA2    /* Play / Pause */
#define DIK_MEDIASTOP       0xA4    /* Media Stop */
#define DIK_VOLUMEDOWN      0xAE    /* Volume - */
#define DIK_VOLUMEUP        0xB0    /* Volume + */
#define DIK_WEBHOME         0xB2    /* Web home */
#define DIK_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
#define DIK_DIVIDE          0xB5    /* / on numeric keypad */
#define DIK_SYSRQ           0xB7
#define DIK_RMENU           0xB8    /* right Alt */
#define DIK_PAUSE           0xC5    /* Pause */
#define DIK_HOME            0xC7    /* Home on arrow keypad */
#define DIK_UP              0xC8    /* UpArrow on arrow keypad */
#define DIK_PRIOR           0xC9    /* PgUp on arrow keypad */
#define DIK_LEFT            0xCB    /* LeftArrow on arrow keypad */
#define DIK_RIGHT           0xCD    /* RightArrow on arrow keypad */
#define DIK_END             0xCF    /* End on arrow keypad */
#define DIK_DOWN            0xD0    /* DownArrow on arrow keypad */
#define DIK_NEXT            0xD1    /* PgDn on arrow keypad */
#define DIK_INSERT          0xD2    /* Insert on arrow keypad */
#define DIK_DELETE          0xD3    /* Delete on arrow keypad */
#define DIK_LWIN            0xDB    /* Left Windows key */
#define DIK_RWIN            0xDC    /* Right Windows key */
#define DIK_APPS            0xDD    /* AppMenu key */
#define DIK_POWER           0xDE    /* System Power */
#define DIK_SLEEP           0xDF    /* System Sleep */
#define DIK_WAKE            0xE3    /* System Wake */
#define DIK_WEBSEARCH       0xE5    /* Web Search */
#define DIK_WEBFAVORITES    0xE6    /* Web Favorites */
#define DIK_WEBREFRESH      0xE7    /* Web Refresh */
#define DIK_WEBSTOP         0xE8    /* Web Stop */
#define DIK_WEBFORWARD      0xE9    /* Web Forward */
#define DIK_WEBBACK         0xEA    /* Web Back */
#define DIK_MYCOMPUTER      0xEB    /* My Computer */
#define DIK_MAIL            0xEC    /* Mail */
#define DIK_MEDIASELECT     0xED    /* Media Select */

 /*
  *  Alternate names for keys, to facilitate transition from DOS.
  */
#define DIK_BACKSPACE       DIK_BACK            /* backspace */
#define DIK_NUMPADSTAR      DIK_MULTIPLY        /* * on numeric keypad */
#define DIK_LALT            DIK_LMENU           /* left Alt */
#define DIK_CAPSLOCK        DIK_CAPITAL         /* CapsLock */
#define DIK_NUMPADMINUS     DIK_SUBTRACT        /* - on numeric keypad */
#define DIK_NUMPADPLUS      DIK_ADD             /* + on numeric keypad */
#define DIK_NUMPADPERIOD    DIK_DECIMAL         /* . on numeric keypad */
#define DIK_NUMPADSLASH     DIK_DIVIDE          /* / on numeric keypad */
#define DIK_RALT            DIK_RMENU           /* right Alt */
#define DIK_UPARROW         DIK_UP              /* UpArrow on arrow keypad */
#define DIK_PGUP            DIK_PRIOR           /* PgUp on arrow keypad */
#define DIK_LEFTARROW       DIK_LEFT            /* LeftArrow on arrow keypad */
#define DIK_RIGHTARROW      DIK_RIGHT           /* RightArrow on arrow keypad */
#define DIK_DOWNARROW       DIK_DOWN            /* DownArrow on arrow keypad */
#define DIK_PGDN            DIK_NEXT            /* PgDn on arrow keypad */
#pragma endregion


using namespace DirectX;

#pragma region GlobalVars
HINSTANCE g_hInst = NULL;
HWND g_hWnd = NULL;
char g_WindowName[100] = "CGP600 - AE2";
D3D_DRIVER_TYPE g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device* g_pD3DDevice = NULL;
ID3D11DeviceContext* g_pImmediateContext = NULL;
IDXGISwapChain* g_pSwapChain = NULL;
ID3D11RenderTargetView* g_pBackBufferRTView = NULL;
ID3D11DepthStencilView* g_pZBuffer;
ID3D11DepthStencilState* g_pDepthStateTrue;
ID3D11DepthStencilState* g_pDepthStateFalse;
Camera* g_camera;
Text2D* g_2DText;
ID3D11BlendState* g_pAlphaBlendEnable;
ID3D11BlendState* g_pAlphaBlendDisable;
ID3D11RasterizerState* rastStateCullNone;
ID3D11RasterizerState* rastStateCullBack;

IDirectInput8* m_pDirectInput;
IDirectInputDevice8* m_pKeyboardDevice;
unsigned char m_keyboardKeyStates[256];

Model* g_pFloor;
Model* g_pPlayer;
Model* g_pEnemy;
#pragma endregion

#pragma region ForwardDeclarations
HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT InitialiseD3D();
void ShutdownD3D();
void RenderFrame(void);
HRESULT InitialiseGraphics(void);
HRESULT Initialise();
void ReadInputStates();
bool IsKeyPressed(unsigned char _keycode) { return m_keyboardKeyStates[_keycode] & 0x80; }
void Input();

#pragma endregion

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (FAILED(InitialiseWindow(hInstance, nCmdShow)))
    {
        DXTRACE_MSG("Failed to create window");
        return 0;
    }

    if (FAILED(Initialise()))
    {
        DXTRACE_MSG("Failed to initialise Input");
        return 0;
    }

    if (FAILED(InitialiseD3D()))
    {
        DXTRACE_MSG("Failed to create device");
        return 0;
    }

    if (FAILED(InitialiseGraphics()))
    {
        DXTRACE_MSG("Failed to initialise graphics");
        return 0;
    }


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
            Input();
            RenderFrame();
        }
    }

    ShutdownD3D();

    return (int)msg.wParam;
}

HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow)
{
    char Name[100] = "CGP600 - AE2 Game\0";

    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);

    // Needed for non-3D apps
    //wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    
    wcex.lpszClassName = Name;

    if (!RegisterClassEx(&wcex)) { return E_FAIL; }

    g_hInst = hInstance;
    RECT rc = { 0, 0, 640, 480 };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    g_hWnd = CreateWindow(Name, g_WindowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                          rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);

    if (!g_hWnd) { return E_FAIL; }
    
    ShowWindow(g_hWnd, nCmdShow);

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

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

HRESULT InitialiseD3D()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(g_hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

    D3D_DRIVER_TYPE driverTypes[] =
    { 
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE, 
    };

    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = true;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                                           D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pD3DDevice, &g_featureLevel, &g_pImmediateContext);

        if (SUCCEEDED(hr)) { break; }
    }

    if (FAILED(hr)) { return hr; }

    ID3D11Texture2D *pBackBufferTexture;
    hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                                (LPVOID*)&pBackBufferTexture);

    if (FAILED(hr)) { return hr; }

    hr = g_pD3DDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &g_pBackBufferRTView);
    pBackBufferTexture->Release();

    if (FAILED(hr)) { return hr; }

    D3D11_TEXTURE2D_DESC tex2dDesc;
    ZeroMemory(&tex2dDesc, sizeof(tex2dDesc));

    tex2dDesc.Width = width;
    tex2dDesc.Height = height;
    tex2dDesc.ArraySize = 1;
    tex2dDesc.MipLevels = 1;
    tex2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    tex2dDesc.SampleDesc.Count = sd.SampleDesc.Count;
    tex2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    tex2dDesc.Usage = D3D11_USAGE_DEFAULT;
    
    ID3D11Texture2D *pZBufferTexture;
    hr = g_pD3DDevice->CreateTexture2D(&tex2dDesc, NULL, &pZBufferTexture);

    if (FAILED(hr)) { return hr; }

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    ZeroMemory(&dsvDesc, sizeof(dsvDesc));

    dsvDesc.Format = tex2dDesc.Format;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    g_pD3DDevice->CreateDepthStencilView(pZBufferTexture, &dsvDesc, &g_pZBuffer);
    pZBufferTexture->Release();

    D3D11_DEPTH_STENCIL_DESC depthDesc;
    ZeroMemory(&depthDesc, sizeof(depthDesc));
    depthDesc.DepthEnable = true;
    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
    
    hr = g_pD3DDevice->CreateDepthStencilState(&depthDesc, &g_pDepthStateTrue);
    if (FAILED(hr)) { return hr; }
    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;


    hr = g_pD3DDevice->CreateDepthStencilState(&depthDesc, &g_pDepthStateFalse);
    if (FAILED(hr)) { return hr; }

    g_pImmediateContext->OMSetRenderTargets(1, &g_pBackBufferRTView, g_pZBuffer);

    D3D11_VIEWPORT viewport;

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (FLOAT)width;
    viewport.Height = (FLOAT)height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    g_pImmediateContext->RSSetViewports(1, &viewport);

    g_2DText = new Text2D("assets/font1.png", g_pD3DDevice, g_pImmediateContext);

    D3D11_BLEND_DESC b;
    b.RenderTarget[0].BlendEnable = TRUE;
    b.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    b.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC1_ALPHA;
    b.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    b.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    b.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    b.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    b.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    b.IndependentBlendEnable = FALSE;
    b.AlphaToCoverageEnable = FALSE;

    g_pD3DDevice->CreateBlendState(&b, &g_pAlphaBlendEnable);

    D3D11_RASTERIZER_DESC rdesc;
    ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
    rdesc.FillMode = D3D11_FILL_SOLID;
    rdesc.CullMode = D3D11_CULL_NONE;
    rdesc.FrontCounterClockwise = false;
    rdesc.DepthBias = false;
    rdesc.DepthBiasClamp = 0;
    rdesc.SlopeScaledDepthBias = 0;
    rdesc.DepthClipEnable = true;
    rdesc.MultisampleEnable = true;
    rdesc.AntialiasedLineEnable = true;

    hr = g_pD3DDevice->CreateRasterizerState(&rdesc, &rastStateCullNone);
    ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
    rdesc.FillMode = D3D11_FILL_SOLID;
    rdesc.CullMode = D3D11_CULL_NONE;
    rdesc.FrontCounterClockwise = false;
    rdesc.DepthBias = false;
    rdesc.DepthBiasClamp = 0;
    rdesc.SlopeScaledDepthBias = 0;
    rdesc.DepthClipEnable = true;
    rdesc.MultisampleEnable = false;
    rdesc.AntialiasedLineEnable = false;
    hr = g_pD3DDevice->CreateRasterizerState(&rdesc, &rastStateCullBack);

    g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return S_OK;
}

HRESULT InitialiseGraphics()
{
    HRESULT hr = S_OK;

    GameObject* m_pGameObject = new GameObject();

    m_pGameObject->GetGraphicsComponent()->SetModel(g_pD3DDevice, g_pImmediateContext, (char*)"assets/cube.obj");
    m_pGameObject->GetGraphicsComponent()->SetTexture(g_pD3DDevice, (char*)"assets/BoxTexture.bmp");

    g_camera = new Camera(0.0f, 0.0f, -5.0f, 0.0f);

    g_pFloor = new Model(g_pD3DDevice, g_pImmediateContext, (char*)"assets/cube.obj");
    g_pFloor->InitObjModel();
    g_pFloor->SetScale(5);
    g_pFloor->SetYPos(-10);

    g_pPlayer = new Model(g_pD3DDevice, g_pImmediateContext, (char*)"assets/cube.obj");
    g_pPlayer->AddTexture((char*)"assets/BoxTexture.bmp");
    g_pPlayer->InitObjModel();
    g_pPlayer->SetXPos(-2);

    g_pEnemy = new Model(g_pD3DDevice, g_pImmediateContext, (char*)"assets/Sphere.obj");
    g_pEnemy->AddTexture((char*)"assets/BoxTexture.bmp");
    g_pEnemy->InitObjModel();
    g_pEnemy->SetScale(0.5);
    g_pEnemy->SetXPos(2);

    return hr;
}

void ShutdownD3D()
{
    if (g_pEnemy) { delete g_pEnemy; }
    if (g_pPlayer) { delete g_pPlayer; }
    if (g_pFloor) { delete g_pFloor; }
    if (g_camera) { delete g_camera; }
    if (g_2DText) { delete g_2DText; };
    if (rastStateCullBack) { rastStateCullBack->Release(); }
    if (rastStateCullNone) { rastStateCullNone->Release(); }
    if (g_pAlphaBlendDisable) { g_pAlphaBlendDisable->Release(); }
    if (g_pAlphaBlendEnable) { g_pAlphaBlendEnable->Release(); }
    if (g_pDepthStateFalse) { g_pDepthStateFalse->Release(); }
    if (g_pDepthStateTrue) { g_pDepthStateTrue->Release(); }
    if (g_pZBuffer) { g_pZBuffer->Release(); }
    if (g_pBackBufferRTView) { g_pBackBufferRTView->Release(); }
    if (g_pSwapChain) { g_pSwapChain->Release(); }
    if (g_pImmediateContext) { g_pImmediateContext->Release(); }
    if (g_pD3DDevice) { g_pD3DDevice->Release(); }
}

void RenderFrame(void)
{
    g_pImmediateContext->OMSetDepthStencilState(g_pDepthStateTrue, 1);
    float rgba_clear_colour[4] = { 0.1f, 0.2f, 0.6f, 1.0f };
    g_pImmediateContext->ClearRenderTargetView(g_pBackBufferRTView, rgba_clear_colour);
    g_pImmediateContext->ClearDepthStencilView(g_pZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    XMMATRIX world, view, projection;
    view = g_camera->GetViewMatrix();
    projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(90), 640.0 / 480.0, 1.0, 100.0);

    g_2DText->AddText("Some Text", -1.0, +1.0, .2);
    g_pImmediateContext->OMSetBlendState(g_pAlphaBlendEnable, 0, 0xffffffff);
    g_pImmediateContext->OMSetBlendState(g_pAlphaBlendDisable, 0, 0xffffffff);

    g_pImmediateContext->RSSetState(rastStateCullBack);

    g_pPlayer->LookAtXZ(g_camera->GetXPos(), g_camera->GetZPos());
    g_pPlayer->MoveForward(0.001f);

    g_pFloor->Draw(&view, &projection);
    g_pPlayer->Draw(&view, &projection);
    g_pEnemy->Draw(&view, &projection);

    if (g_pPlayer->CheckCollision(g_pEnemy)) { g_pPlayer->IncXPos(0); }

    //world = XMMatrixIdentity();
    //g_pRootNode->Execute(&world, &view, &projection);

    g_pImmediateContext->RSSetState(rastStateCullNone);
    g_2DText->RenderText();
    g_pSwapChain->Present(0, 0);
}

HRESULT Initialise()
{
    HRESULT hr;
    ZeroMemory(m_keyboardKeyStates, sizeof(m_keyboardKeyStates));
    hr = DirectInput8Create(g_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);

    if (FAILED(hr)) { return hr; }

    hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboardDevice, NULL);

    if (FAILED(hr)) { return hr; }

    hr = m_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
    if (FAILED(hr)) return hr;

    hr = m_pKeyboardDevice->SetCooperativeLevel(g_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

    if (FAILED(hr)) { return hr; }

    hr = m_pKeyboardDevice->Acquire();

    if (FAILED(hr)) { return hr; }

    return S_OK;
}

void ReadInputStates()
{
    HRESULT hr;
    hr = m_pKeyboardDevice->GetDeviceState(sizeof(m_keyboardKeyStates), (LPVOID)&m_keyboardKeyStates);

    if (FAILED(hr)) { if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) { m_pKeyboardDevice->Acquire(); } }
}

void Input()
{
    ReadInputStates();
    if (IsKeyPressed(DIK_ESCAPE)) { DestroyWindow(g_hWnd); }
    //if (IsKeyPressed(DIK_A)) { g_camera->Strafe(-0.001f); }
    //if (IsKeyPressed(DIK_D)) { g_camera->Strafe(0.001f); }
    //if (IsKeyPressed(DIK_W)) { g_camera->Forward(0.001f); }
    //if (IsKeyPressed(DIK_S)) { g_camera->Forward(-0.001f); }

    if (IsKeyPressed(DIK_A)) { g_camera->Strafe(0.0025f); }
    if (IsKeyPressed(DIK_D)) { g_camera->Strafe(-0.0025f); }
    if (IsKeyPressed(DIK_W)) { g_camera->Forward(0.05f); }
    if (IsKeyPressed(DIK_S)) { g_camera->Forward(-0.05f); }
}