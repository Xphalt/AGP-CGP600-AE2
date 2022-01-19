#pragma once
#include "InitDirectX.h"

InitDirectX InitDirectX::s_instance;

HRESULT InitDirectX::Initialise()
{
    HRESULT hr{S_OK};

    InitialiseDirectX(hr);

    InitialiseShaders();

    InitialiseScene(hr);

    return S_OK;
}

HRESULT InitDirectX::InitialiseDirectX(HRESULT& hr)
{
#pragma region Initialise drivers
    RECT rc;
    GetClientRect(Renderer::GetInstance().GetHWND(), &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

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
#pragma endregion

#pragma region Create and set device and swap chain
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(scd));

    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.BufferCount = 1;
    scd.OutputWindow = Renderer::GetInstance().m_hwnd;
    scd.Windowed = TRUE;

    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;



    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        m_driverType = driverTypes[driverTypeIndex];

        hr = D3D11CreateDeviceAndSwapChain
        (
            NULL,
            m_driverType,
            NULL,
            createDeviceFlags,
            featureLevels,
            numFeatureLevels,
            D3D11_SDK_VERSION,
            &scd,
            &Renderer::GetInstance().m_pSwapChain,
            &Renderer::GetInstance().m_pDevice,
            &m_featureLevel,
            &Renderer::GetInstance().m_pDeviceContext
        );

        if (SUCCEEDED(hr)) { break; }
    }

    if (FAILED(hr)) { return hr; }
#pragma endregion

#pragma region Create and set back buffer
    ID3D11Texture2D* pBackBuffer{nullptr};
    hr = Renderer::GetInstance().m_pSwapChain->GetBuffer
    (
        0,
        __uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(&pBackBuffer)
    );

    if (FAILED(hr)) { return hr; }
#pragma endregion

#pragma region Create and set render target view
    hr = Renderer::GetInstance().m_pDevice->CreateRenderTargetView
    (
        pBackBuffer,
        NULL,
        &Renderer::GetInstance().m_pRenderTargetView
    );

    if (FAILED(hr)) { return hr; }
#pragma endregion

#pragma region Create and set depth stencil
    D3D11_TEXTURE2D_DESC depthStencilDesc{NULL};
    depthStencilDesc.Width = width;
    depthStencilDesc.Height = height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    hr = Renderer::GetInstance().m_pDevice->CreateTexture2D(&depthStencilDesc, NULL, &Renderer::GetInstance().m_pDSB);

    if (FAILED(hr)) { return hr; }

    hr = Renderer::GetInstance().m_pDevice->CreateDepthStencilView(Renderer::GetInstance().m_pDSB, NULL, &Renderer::GetInstance().m_pDSV);

    if (FAILED(hr)) { return hr; }

    Renderer::GetInstance().m_pDeviceContext->OMSetRenderTargets(1, &Renderer::GetInstance().m_pRenderTargetView, Renderer::GetInstance().m_pDSV);

    D3D11_DEPTH_STENCIL_DESC depthStencildesc;
    ZeroMemory(&depthStencildesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

    depthStencildesc.DepthEnable = true;
    depthStencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

    hr = Renderer::GetInstance().m_pDevice->CreateDepthStencilState(&depthStencildesc, &Renderer::GetInstance().m_pDSS);

    if (FAILED(hr)) { return hr; }
#pragma endregion

#pragma region Create and set viewport
    D3D11_VIEWPORT viewport{NULL};

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (FLOAT)width;
    viewport.Height = (FLOAT)height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    Renderer::GetInstance().m_pDeviceContext->RSSetViewports(1, &viewport);
#pragma endregion

#pragma region Create and set rasterizer state
    D3D11_RASTERIZER_DESC rastDesc;
    ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
    rastDesc.FillMode = D3D11_FILL_SOLID;
    rastDesc.CullMode = D3D11_CULL_BACK;

    hr = Renderer::GetInstance().GetDevice()->CreateRasterizerState(&rastDesc, &Renderer::GetInstance().m_pRasterState);

    if (FAILED(hr)) { return hr; }
#pragma endregion

#pragma region Create and set sampler state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    hr = Renderer::GetInstance().m_pDevice->CreateSamplerState(&sampDesc, &Renderer::GetInstance().m_pSamplerState);

    if (FAILED(hr)) { return hr; }
#pragma endregion

    return S_OK;
}

void InitDirectX::InitialiseShaders()
{
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    UINT numElements = ARRAYSIZE(layout);

    Renderer::GetInstance().m_pShaders->InitialiseVertexShader(Renderer::GetInstance().m_pDevice, L"assets/shaders/VertexShader.cso", layout, numElements);
    Renderer::GetInstance().m_pShaders->InitialisePixelShader(Renderer::GetInstance().m_pDevice, L"assets/shaders/PixelShader.cso");
}

HRESULT InitDirectX::InitialiseScene(HRESULT& hr)
{
    Vertex vertex[] =
    {
        Vertex(-0.5f,  -0.5f, 1.0f, 0.0f, 1.0f), //Bottom Left  - [0]
        Vertex(-0.5f,   0.5f, 1.0f, 0.0f, 0.0f), //Top Left     - [1]
        Vertex(0.5f,   0.5f, 1.0f, 1.0f, 0.0f),  //Top Right    - [2]
        Vertex(0.5f, -0.5f, 1.0f, 1.0f, 1.0f),  //Bottom Left  - [3]
    };

    DWORD indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    hr = Renderer::GetInstance().m_pVertexBuffer.Initialise(Renderer::GetInstance().m_pDevice, vertex, ARRAYSIZE(vertex));

    if (FAILED(hr)) { return hr; }

    hr = Renderer::GetInstance().m_pIndicesBuffer.Initialise(Renderer::GetInstance().m_pDevice, indices, ARRAYSIZE(indices));

    if (FAILED(hr)) { return hr; }

    hr = D3DX11CreateShaderResourceViewFromFile(Renderer::GetInstance().m_pDevice, "BoxTexture.bmp", NULL, NULL, &Renderer::GetInstance().m_pTexture, NULL);

    if (FAILED(hr)) { return hr; }

    hr = Renderer::GetInstance().m_pConstantBuffer.Initialize(Renderer::GetInstance().m_pDevice, Renderer::GetInstance().m_pDeviceContext);

    if (FAILED(hr)) { return hr; }
    
    return S_OK;
}