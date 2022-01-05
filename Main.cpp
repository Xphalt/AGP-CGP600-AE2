#pragma region Includes
// IMPORTANT: Includes must be in this order:
#include <d3d11.h>
#include <dxgi.h>
#include <D3DX11.h>
#include <windows.h>
#include <dxerr.h>
#include "Camera.h"
#include "text2D.h"

/* Use these defines when using DirectXMath
   Make sure they are before the include */
#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <DirectXMath.h>

using namespace DirectX;
#pragma endregion

#pragma region GlobalVars
// Define vertex structure
struct POS_COL_TEX_VERTEX
{
    XMFLOAT3 pos;
    XMFLOAT4 col;
    XMFLOAT2 texture0;
};

struct CONSTANT_BUFFER0
{
    // 64 bytes
    XMMATRIX WorldViewProjection;
    // 4 bytes
    float RedAmount;
    // 4 bytes
    float scale;
    // 2 x 4 bytes = 8 bytes
    XMFLOAT2 packing_bytes;

    // TOTAL SIZE = 80 bytes
};

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
ID3D11Buffer* g_pConstantBuffer0;
CONSTANT_BUFFER0 cb0_values;
ID3D11DepthStencilView* g_pZBuffer;
Camera* g_camera;
ID3D11ShaderResourceView* g_pTexture0;
ID3D11SamplerState* g_pSampler0;
Text2D* g_2DText;
ID3D11BlendState* g_pAlphaBlendEnable;
ID3D11BlendState* g_pAlphaBlendDisable;
ID3D11RasterizerState* rastStateCullNone;
ID3D11RasterizerState* rastStateCullBack;
#pragma endregion

#pragma region ForwardDeclarations
HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT InitialiseD3D();
void ShutdownD3D();
void RenderFrame(void);
HRESULT InitialiseGraphics(void);
#pragma endregion

#pragma region EntryPoint
// Entry point
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (FAILED(InitialiseWindow(hInstance, nCmdShow)))
    {
        DXTRACE_MSG("Failed to create window");
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

    // Main message loop
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
            RenderFrame();
        }
    }

    ShutdownD3D();

    return (int)msg.wParam;
}
#pragma endregion

#pragma region CreateWindow
// Register class and create window
HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow)
{
    // Give app name
    char Name[100] = "CGP600 - AE2 Game\0";

    // Register class
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

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, 640, 480 };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    g_hWnd = CreateWindow(Name, g_WindowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                          rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);

    if (!g_hWnd) { return E_FAIL; }
    
    ShowWindow(g_hWnd, nCmdShow);

    return S_OK;
}
#pragma endregion

#pragma region RecieveMessage
// Called every time the app recieves a message
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
#pragma endregion

#pragma region InitialiseD3D
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
        /* Comment out this line if you need functionallity on hardware
        that doesn't support it */
        D3D_DRIVER_TYPE_HARDWARE,

        /* Comment out this line to use reference device */
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

    // Get pointer to back buffer texture
    ID3D11Texture2D *pBackBufferTexture;
    hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                                (LPVOID*)&pBackBufferTexture);

    if (FAILED(hr)) { return hr; }

    // Use the back buffer texture pointer to create the render target view
    hr = g_pD3DDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &g_pBackBufferRTView);
    pBackBufferTexture->Release();

    if (FAILED(hr)) { return hr; }

    // Create a Z buffer texture
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

    // Create the Z buffer
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    ZeroMemory(&dsvDesc, sizeof(dsvDesc));

    dsvDesc.Format = tex2dDesc.Format;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    g_pD3DDevice->CreateDepthStencilView(pZBufferTexture, &dsvDesc, &g_pZBuffer);
    pZBufferTexture->Release();

    // Set the render target view
    g_pImmediateContext->OMSetRenderTargets(1, &g_pBackBufferRTView, g_pZBuffer);

    // Set the viewport
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

    return S_OK;
}
#pragma endregion

#pragma region InitialiseGraphics
HRESULT InitialiseGraphics()
{
    HRESULT hr = S_OK;

#pragma region Cube
    // Define vertices of a triangle - screen coordinates -1.0 to +1.0
    POS_COL_TEX_VERTEX vertices[] =
    {
        // Back face
        {XMFLOAT3(-1.0f, 1.0f, 1.0f),   XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
        {XMFLOAT3(-1.0f, -1.0f, 1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
        {XMFLOAT3(1.0f, 1.0f, 1.0f),    XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
        {XMFLOAT3(1.0f, 1.0f, 1.0f),    XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
        {XMFLOAT3(-1.0f, -1.0f, 1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
        {XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},

        // Front face
        {XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
        {XMFLOAT3(-1.0f, 1.0f, -1.0f),  XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
        {XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
        {XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
        {XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
        {XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},
                                        
        // Left face                    
        {XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
        {XMFLOAT3(-1.0f, -1.0f, 1.0f),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
        {XMFLOAT3(-1.0f, 1.0f, -1.0f),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},
        {XMFLOAT3(-1.0f, -1.0f, 1.0f),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
        {XMFLOAT3(-1.0f, 1.0f, 1.0f),   XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
        {XMFLOAT3(-1.0f, 1.0f, -1.0f),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},

        // Right face
        {XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
        {XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
        {XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},
        {XMFLOAT3(1.0f, 1.0f, 1.0f),    XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
        {XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
        {XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},

        // Bottom face
        {XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},
        {XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
        {XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
        {XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
        {XMFLOAT3(-1.0f, -1.0f, 1.0f),  XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
        {XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},

        // Top face
        {XMFLOAT3(1.0f, 1.0f, 1.0f),    XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
        {XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},
        {XMFLOAT3(-1.0f, 1.0f, -1.0f),  XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
        {XMFLOAT3(-1.0f, 1.0f, 1.0f),   XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
        {XMFLOAT3(1.0f, 1.0f, 1.0f),    XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
        {XMFLOAT3(-1.0f, 1.0f, -1.0f),  XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)}
        #pragma endregion
    };

    D3DX11CreateShaderResourceViewFromFile(g_pD3DDevice, "assets/BoxTexture.bmp", NULL, NULL, &g_pTexture0, NULL);

    // Set up and create vertex buffer
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = sizeof(vertices);
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    // Set up and create the sample state
    D3D11_SAMPLER_DESC sampler_desc;
    ZeroMemory(&sampler_desc, sizeof(sampler_desc));
    sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

    g_pD3DDevice->CreateSamplerState(&sampler_desc, &g_pSampler0);

    // Create the buffer
    hr = g_pD3DDevice->CreateBuffer(&bufferDesc, NULL, &g_pVertexBuffer);
    
    if (FAILED(hr)) { return hr; }

    // Create constant buffer
    D3D11_BUFFER_DESC constant_buffer_desc;
    ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

    // Can use UpdateSubresource() to update
    constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    // MUST be a multiple of 16, calculate from CB struct
    constant_buffer_desc.ByteWidth = 80;
    // Use as a constant buffer
    constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    hr = g_pD3DDevice->CreateBuffer(&constant_buffer_desc, NULL, &g_pConstantBuffer0);

    if (FAILED(hr)) { return hr; }

    // Copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;

    // Lock the buffer to allow writing
    g_pImmediateContext->Map(g_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

    // Copy the data
    memcpy(ms.pData, vertices, sizeof(vertices));

    // Unlock the buffer
    g_pImmediateContext->Unmap(g_pVertexBuffer, NULL);

    // Load and compile the pixel + vertex shaders - use vs_5_0 to target DX11 hardware only
    ID3DBlob *VS, *PS, *error;
    hr = D3DX11CompileFromFile("shaders.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, &error, 0);

    // Check for shader compilation error
    if (error != 0)
    {
        OutputDebugStringA((char*)error->GetBufferPointer());
        error->Release();

        // Don't fail if error is just warning
        if (FAILED(hr)) { return hr; }
    }

    hr = D3DX11CompileFromFile("shaders.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, &error, 0);

    // Check for shader compilation error
    if (error != 0)
    {
        OutputDebugStringA((char*)error->GetBufferPointer());
        error->Release();

        // Don't fail if error is just a warning
        if (FAILED(hr)) { return hr; }
    }

    // Create shader objects
    hr = g_pD3DDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &g_pVertexShader);

    if (FAILED(hr)) { return hr; }

    hr = g_pD3DDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &g_pPixelShader);

    if (FAILED(hr)) { return hr; }

    // Create and set the input layout object
    D3D11_INPUT_ELEMENT_DESC iedesc[] =
    {
        // Be very careful setting the correct dxgi format and D3D version
        // Note the spelling of COLOR
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    hr = g_pD3DDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &g_pInputLayout);

    if (FAILED(hr)) { return hr; }

    // 50% of vertex red value
    cb0_values.RedAmount = 0.5f;

    g_camera = new Camera(0.0, 0.0, -0.5, 0);

    return S_OK;
}
#pragma endregion

#pragma region ShutdownD3D
void ShutdownD3D()
{

    if (g_2DText) { delete g_2DText; };
    if (g_pTexture0) { g_pTexture0->Release(); }
    if (g_pSampler0) { g_pSampler0->Release(); }
    if (g_camera) { delete g_camera; }
    if (g_pZBuffer) { g_pZBuffer->Release(); }
    if (g_pConstantBuffer0) { g_pConstantBuffer0->Release(); }
    if (g_pVertexBuffer) { g_pVertexBuffer->Release(); }
    if (g_pInputLayout) { g_pInputLayout->Release(); }
    if (g_pVertexShader) { g_pVertexShader->Release(); }
    if (g_pPixelShader) { g_pPixelShader->Release(); }
    if (g_pBackBufferRTView) { g_pBackBufferRTView->Release(); }
    if (g_pSwapChain) { g_pSwapChain->Release(); }
    if (g_pImmediateContext) { g_pImmediateContext->Release(); }
    if (g_pD3DDevice) { g_pD3DDevice->Release(); }
}
#pragma endregion

#pragma region RenderFrame
void RenderFrame(void)
{
    // Clear the back buffer - choose colour
    float rgba_clear_colour[4] = { 0.1f, 0.2f, 0.6f, 1.0f };
    g_pImmediateContext->ClearRenderTargetView(g_pBackBufferRTView, rgba_clear_colour);
    g_pImmediateContext->ClearDepthStencilView(g_pZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // Set vertex buffer // 03 - 01
    UINT stride = sizeof(POS_COL_TEX_VERTEX);
    UINT offset = 0;
    g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // Select which primitive type to use // 03 - 01
    g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer0);
    g_pImmediateContext->VSSetShader(g_pVertexShader, 0, 0);
    g_pImmediateContext->PSSetShader(g_pPixelShader, 0, 0);
    g_pImmediateContext->IASetInputLayout(g_pInputLayout);
    g_camera->GetViewMatrix();

    XMMATRIX projection, world, view, world2;

    world = XMMatrixRotationX(XMConvertToRadians(60));
    world *= XMMatrixTranslation(-10, 0, 20);
    world2 = XMMatrixRotationX(XMConvertToRadians(60));
    world2 *= XMMatrixTranslation(10, 0, 20);
    projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0), 640.0 / 480.0, 1.0, 100.0);
    view = XMMatrixIdentity();
    cb0_values.WorldViewProjection = world * view * projection;
    cb0_values.WorldViewProjection = world2 * view * projection;

    g_pImmediateContext->UpdateSubresource(g_pConstantBuffer0, 0, 0, &cb0_values, 0, 0);

    g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer0);

    // Use texture and sample states
    g_pImmediateContext->PSSetSamplers(0, 1, &g_pSampler0);
    g_pImmediateContext->PSSetShaderResources(0, 1, &g_pTexture0);

    g_pImmediateContext->RSSetState(rastStateCullBack);

    // Draw the vertex buffer to the back buffer // 03 - 01
    g_pImmediateContext->Draw(36, 0);
    g_pImmediateContext->RSSetState(rastStateCullNone);

    g_2DText->AddText("Some Text", -1.0, +1.0, .2);
    g_pImmediateContext->OMSetBlendState(g_pAlphaBlendEnable, 0, 0xffffffff);
    g_2DText->RenderText();
    g_pImmediateContext->OMSetBlendState(g_pAlphaBlendDisable, 0, 0xffffffff);

    // Display what has just been rendered
    g_pSwapChain->Present(0, 0);
}
#pragma endregion