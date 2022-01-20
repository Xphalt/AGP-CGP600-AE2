#include <d3d11.h>
#include <dxgi.h>
#include <D3DX11.h>
#include <windows.h>
#include <dxerr.h>
#include <dinput.h>
#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <DirectXMath.h>
#include "Camera.h"
#include "text2D.h"
#include "Model.h"
//#include "SceneNode.h"

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
ID3D11Buffer* g_pVertexBuffer;
ID3D11VertexShader* g_pVertexShader;
ID3D11PixelShader* g_pPixelShader;
ID3D11InputLayout* g_pInputLayout;
ID3D11DepthStencilView* g_pZBuffer;
ID3D11DepthStencilState* g_pDepthStateTrue;
ID3D11DepthStencilState* g_pDepthStateFalse;
Camera* g_camera;
ID3D11ShaderResourceView* g_pTexture0;
ID3D11SamplerState* g_pSampler0;
Text2D* g_2DText;
ID3D11BlendState* g_pAlphaBlendEnable;
ID3D11BlendState* g_pAlphaBlendDisable;
ID3D11RasterizerState* rastStateCullNone;
ID3D11RasterizerState* rastStateCullBack;
XMVECTOR g_directional_light_shines_from;
XMVECTOR g_directional_light_colour;
XMVECTOR g_ambient_light_colour;
SceneNode* g_pRootNode;
SceneNode* g_pNode1;
SceneNode* g_pNode2;

IDirectInput8* m_pDirectInput;
IDirectInputDevice8* m_pKeyboardDevice;
unsigned char m_keyboardKeyStates[256];

Model* g_pPlayer;
Model* g_pEnemy;
#pragma endregion

#pragma region ForwardDeclarations
HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Initialise(HINSTANCE _hInstance, HWND _hwnd);
void ReadInputStates();
HRESULT InitialiseD3D();
void ShutdownD3D();
void RenderFrame(void);
HRESULT InitialiseGraphics(void);
bool IsKeyPressed(unsigned char _keycode);

#pragma endregion

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (FAILED(InitialiseWindow(g_hInst, nCmdShow)))
    {
        DXTRACE_MSG("Failed to create window");
        return 0;
    }

    Initialise(g_hInst, g_hWnd);

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
            if (IsKeyPressed(DIK_ESCAPE)) { DestroyWindow(g_hWnd); }
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

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            DestroyWindow(g_hWnd);
            return 0;
        }

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

    g_camera = new Camera(0.0f, 0.0f, -5.0f, 0.0f);

    //g_pRootNode = new SceneNode();
    //g_pNode1 = new SceneNode();
    //g_pNode2 = new SceneNode();

    g_pPlayer = new Model(g_pD3DDevice, g_pImmediateContext, (char*)"assets/cube.obj");
    g_pPlayer->AddTexture((char*)"assets/BoxTexture.bmp");
    //g_pNode1->SetModel(g_pPlayer);
    //g_pRootNode->AddChildNode(g_pNode1);

    g_pEnemy = new Model(g_pD3DDevice, g_pImmediateContext, (char*)"assets/Sphere.obj");
    g_pEnemy->AddTexture((char*)"assets/BoxTexture.bmp");
    //g_pNode2->SetModel(g_pEnemy);
    //g_pNode1->AddChildNode(g_pNode2);
    
    g_pPlayer->InitObjModel();
    g_pPlayer->SetXPos(-2);
    g_pEnemy-> InitObjModel();
    g_pEnemy->SetXPos(2);
    g_pEnemy->SetScale(0.5);

    return hr;
}

void ShutdownD3D()
{
    if (g_2DText) { delete g_2DText; };
    if (g_pTexture0) { g_pTexture0->Release(); }
    if (g_pSampler0) { g_pSampler0->Release(); }
    if (g_camera) { delete g_camera; }
    if (g_pZBuffer) { g_pZBuffer->Release(); }
    if (g_pBackBufferRTView) { g_pBackBufferRTView->Release(); }
    if (g_pSwapChain) { g_pSwapChain->Release(); }
    if (g_pImmediateContext) { g_pImmediateContext->Release(); }
    if (g_pPlayer) { delete g_pPlayer; }
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


    g_pPlayer->Draw(&view, &projection);
    g_pEnemy->Draw(&view, &projection);

    //world = XMMatrixIdentity();
    //g_pRootNode->Execute(&world, &view, &projection);

    g_pImmediateContext->RSSetState(rastStateCullNone);
    g_2DText->RenderText();
    g_pSwapChain->Present(0, 0);
}

HRESULT Initialise(HINSTANCE _hInstance, HWND _hwnd)
{
    HRESULT hr;
    ZeroMemory(m_keyboardKeyStates, sizeof(m_keyboardKeyStates));
    hr = DirectInput8Create(_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);

    if (FAILED(hr)) { return hr; }

    hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboardDevice, NULL);

    if (FAILED(hr)) { return hr; }

    hr = m_pKeyboardDevice->SetCooperativeLevel(_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

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

bool IsKeyPressed(unsigned char _keycode) { return m_keyboardKeyStates[_keycode] & 0x80; }