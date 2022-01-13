#pragma once
#include "InitDirectX.h"

InitDirectX::InitDirectX()
{
}

InitDirectX::~InitDirectX()
{
    if (m_pRenderTargetView) { m_pRenderTargetView->Release(); }
    if (m_pSwapChain) { m_pSwapChain->Release(); }
    if (m_pDeviceContext) { m_pDeviceContext->Release(); }
    if (m_pDevice) { m_pDevice->Release(); }
}

HRESULT InitDirectX::Initialise(HWND _hwnd)
{
    HRESULT hr{S_OK};

#pragma region Initialise drivers
    RECT rc;
        GetClientRect(_hwnd, &rc);
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

        scd.BufferCount = 1;
        scd.BufferDesc.Width = width;
        scd.BufferDesc.Height = height;
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.BufferDesc.RefreshRate.Numerator = 60;
        scd.BufferDesc.RefreshRate.Denominator = 1;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.OutputWindow = _hwnd;
        scd.SampleDesc.Count = 1;
        scd.SampleDesc.Quality = 0;
        scd.Windowed = true;

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
            &m_pSwapChain,
            &m_pDevice,
            &m_featureLevel,
            &m_pDeviceContext
        );

        if (SUCCEEDED(hr)) { break; }
    }

    if (FAILED(hr)) { DXTRACE_MSG("Failed to create device and swap chain."); return hr; }
#pragma endregion

#pragma region Create and set back buffer
    ID3D11Texture2D* pBackBuffer;
    hr = m_pSwapChain->GetBuffer
    (
        0, 
        __uuidof(ID3D11Texture2D),
        (LPVOID*)&pBackBuffer
    );

    if (FAILED(hr)) { DXTRACE_MSG("Failed to create back buffer."); return hr; }
#pragma endregion

#pragma region Create and set render target view
    hr = m_pDevice->CreateRenderTargetView
    (
        pBackBuffer,
        NULL,
        &m_pRenderTargetView
    );

    pBackBuffer->Release();

    if (FAILED(hr)) { DXTRACE_MSG("Failed to create render target view."); return hr; }

    m_pDeviceContext->OMSetRenderTargets
    (
        1,
        &m_pRenderTargetView,
        NULL
    );
#pragma endregion

#pragma region Create and set the viewport
    D3D11_VIEWPORT viewport;

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (FLOAT)width;
    viewport.Height = (FLOAT)height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    m_pDeviceContext->RSSetViewports(1, &viewport);
#pragma endregion

    return S_OK;
}

//#pragma region MyRegion
//HRESULT InitDirectX::InitialiseD3D()
//{
//  
//    DXGI_SWAP_CHAIN_DESC sd;
//    ZeroMemory(&sd, sizeof(sd));
//    sd.BufferCount = 1;
//    sd.BufferDesc.Width = width;
//    sd.BufferDesc.Height = height;
//    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//    sd.BufferDesc.RefreshRate.Numerator = 60;
//    sd.BufferDesc.RefreshRate.Denominator = 1;
//    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//    sd.OutputWindow = m_hWnd;
//    sd.SampleDesc.Count = 1;
//    sd.SampleDesc.Quality = 0;
//    sd.Windowed = true;
// 
//    // Create a Z buffer texture
//    D3D11_TEXTURE2D_DESC tex2dDesc;
//    ZeroMemory(&tex2dDesc, sizeof(tex2dDesc));
//
//    tex2dDesc.Width = width;
//    tex2dDesc.Height = height;
//    tex2dDesc.ArraySize = 1;
//    tex2dDesc.MipLevels = 1;
//    tex2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
//    tex2dDesc.SampleDesc.Count = sd.SampleDesc.Count;
//    tex2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
//    tex2dDesc.Usage = D3D11_USAGE_DEFAULT;
//
//    ID3D11Texture2D* pZBufferTexture;
//    hr = m_pD3DDevice->CreateTexture2D(&tex2dDesc, NULL, &pZBufferTexture);
//
//    if (FAILED(hr)) { return hr; }
//
//    // Create the Z buffer
//    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
//    ZeroMemory(&dsvDesc, sizeof(dsvDesc));
//
//    dsvDesc.Format = tex2dDesc.Format;
//    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
//
//    m_pD3DDevice->CreateDepthStencilView(pZBufferTexture, &dsvDesc, &m_pZBuffer);
//    pZBufferTexture->Release();
//
//    // Set the render target view
//    m_pImmediateContext->OMSetRenderTargets(1, &m_pBackBufferRTView, m_pZBuffer);
//
//    //m_2DText = new Text2D("assets/font1.png", m_pD3DDevice, m_pImmediateContext);
//
//    D3D11_BLEND_DESC b;
//    b.RenderTarget[0].BlendEnable = TRUE;
//    b.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
//    b.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC1_ALPHA;
//    b.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
//    b.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
//    b.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
//    b.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
//    b.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
//    b.IndependentBlendEnable = FALSE;
//    b.AlphaToCoverageEnable = FALSE;
//
//    m_pD3DDevice->CreateBlendState(&b, &m_pAlphaBlendEnable);
//
//    D3D11_RASTERIZER_DESC rdesc;
//    ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
//    rdesc.FillMode = D3D11_FILL_SOLID;
//    rdesc.CullMode = D3D11_CULL_NONE;
//    rdesc.FrontCounterClockwise = false;
//    rdesc.DepthBias = false;
//    rdesc.DepthBiasClamp = 0;
//    rdesc.SlopeScaledDepthBias = 0;
//    rdesc.DepthClipEnable = true;
//    rdesc.MultisampleEnable = true;
//    rdesc.AntialiasedLineEnable = true;
//
//    hr = m_pD3DDevice->CreateRasterizerState(&rdesc, &rastStateCullNone);
//    ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
//    rdesc.FillMode = D3D11_FILL_SOLID;
//    rdesc.CullMode = D3D11_CULL_NONE;
//    rdesc.FrontCounterClockwise = false;
//    rdesc.DepthBias = false;
//    rdesc.DepthBiasClamp = 0;
//    rdesc.SlopeScaledDepthBias = 0;
//    rdesc.DepthClipEnable = true;
//    rdesc.MultisampleEnable = false;
//    rdesc.AntialiasedLineEnable = false;
//    hr = m_pD3DDevice->CreateRasterizerState(&rdesc, &rastStateCullBack);
//
//    return S_OK;
//}

//HRESULT InitDirectX::InitialiseGraphics()
//{
//    HRESULT hr = S_OK;
//
//    //m_model = new Model(m_pD3DDevice, m_pImmediateContext);
//
//    //hr = m_model->LoadObjModel((char*)"assets/Sphere.obj");
//
//#pragma region Cube
//    // Define vertices of a triangle - screen coordinates -1.0 to +1.0
//    POS_COL_TEX_NORM_VERTEX vertices[] =
//    {
//        // Back face
//        {XMFLOAT3(-1.0f, 1.0f, 1.0f),   XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
//        {XMFLOAT3(-1.0f, -1.0f, 1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
//        {XMFLOAT3(1.0f, 1.0f, 1.0f),    XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
//        {XMFLOAT3(1.0f, 1.0f, 1.0f),    XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
//        {XMFLOAT3(-1.0f, -1.0f, 1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
//        {XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
//
//        // Front face                                                                         
//        {XMFLOAT3(-1.0f, 1.0f, -1.0f),  XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
//        {XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
//        {XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
//        {XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
//        {XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
//        {XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
//
//        // Left face                                                                          
//        {XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)},
//        {XMFLOAT3(-1.0f, -1.0f, 1.0f),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)},
//        {XMFLOAT3(-1.0f, 1.0f, -1.0f),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)},
//        {XMFLOAT3(-1.0f, -1.0f, 1.0f),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)},
//        {XMFLOAT3(-1.0f, 1.0f, 1.0f),   XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)},
//        {XMFLOAT3(-1.0f, 1.0f, -1.0f),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f)},
//
//        // Right face                                                                         
//        {XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
//        {XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
//        {XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
//        {XMFLOAT3(1.0f, 1.0f, 1.0f),    XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
//        {XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
//        {XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
//
//        // Bottom face                                                                        
//        {XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)},
//        {XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)},
//        {XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)},
//        {XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)},
//        {XMFLOAT3(-1.0f, -1.0f, 1.0f),  XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)},
//        {XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f)},
//
//        // Top face                                                                           
//        {XMFLOAT3(1.0f, 1.0f, 1.0f),    XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
//        {XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
//        {XMFLOAT3(-1.0f, 1.0f, -1.0f),  XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
//        {XMFLOAT3(-1.0f, 1.0f, 1.0f),   XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
//        {XMFLOAT3(1.0f, 1.0f, 1.0f),    XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
//        {XMFLOAT3(-1.0f, 1.0f, -1.0f),  XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)}
//    };
//#pragma endregion
//
//#pragma region Create and set buffers and sampler
//    // Create and setup vertex buffer
//    D3D11_BUFFER_DESC bufferDesc;
//    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
//    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//    bufferDesc.ByteWidth = sizeof(vertices);
//    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//    hr = m_pD3DDevice->CreateBuffer(&bufferDesc, NULL, &m_pVertexBuffer);
//
//    if (FAILED(hr)) { return hr; }
//
//    // Create and setup constant buffer
//    D3D11_BUFFER_DESC constant_buffer_desc;
//    ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));
//    constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
//    constant_buffer_desc.ByteWidth = 112;
//    constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//    hr = m_pD3DDevice->CreateBuffer(&constant_buffer_desc, NULL, &m_pConstantBuffer0);
//
//    if (FAILED(hr)) { return hr; }
//
//    // Create and setup the sample state
//    D3D11_SAMPLER_DESC sampler_desc;
//    ZeroMemory(&sampler_desc, sizeof(sampler_desc));
//    sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//    sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//    sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//    sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//    sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
//
//    m_pD3DDevice->CreateSamplerState(&sampler_desc, &m_pSampler0);
//
//    D3D11_MAPPED_SUBRESOURCE ms;
//    m_pImmediateContext->Map(m_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
//    memcpy(ms.pData, vertices, sizeof(vertices));
//    m_pImmediateContext->Unmap(m_pVertexBuffer, NULL);
//#pragma endregion
//
//#pragma region Load and compile shaders
//    ID3DBlob* VS, * PS, * error;
//
//    // Vertex Shader
//    hr = D3DX11CompileFromFile("shaders.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, &error, 0);
//
//    if (error != 0)
//    {
//        OutputDebugStringA((char*)error->GetBufferPointer());
//        error->Release();
//
//        if (FAILED(hr)) { return hr; }
//    }
//
//    hr = m_pD3DDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pVertexShader);
//
//    if (FAILED(hr)) { return hr; }
//
//    // Pixel Shader
//    hr = D3DX11CompileFromFile("shaders.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, &error, 0);
//
//    if (error != 0)
//    {
//        OutputDebugStringA((char*)error->GetBufferPointer());
//        error->Release();
//
//        if (FAILED(hr)) { return hr; }
//    }
//
//    hr = m_pD3DDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pPixelShader);
//
//    if (FAILED(hr)) { return hr; }
//#pragma endregion
//
//#pragma region Create and set input layout
//    D3D11_INPUT_ELEMENT_DESC iedesc[] =
//    {
//        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
//        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
//        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
//        {"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
//    };
//
//    hr = m_pD3DDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);
//
//    if (FAILED(hr)) { return hr; }
//#pragma endregion
//
//    //m_camera = new Camera(0.0f, 0.0f, -0.5f, 0.0f);
//
//    return S_OK;
//
//}
//
//void InitDirectX::Shutdown()
//{
//    if (m_pTexture0) { m_pTexture0->Release(); }
//    if (m_pSampler0) { m_pSampler0->Release(); }
//    if (m_pZBuffer) { m_pZBuffer->Release(); }
//    if (m_pConstantBuffer0) { m_pConstantBuffer0->Release(); }
//    if (m_pVertexBuffer) { m_pVertexBuffer->Release(); }
//    if (m_pInputLayout) { m_pInputLayout->Release(); }
//    if (m_pVertexShader) { m_pVertexShader->Release(); }
//    if (m_pPixelShader) { m_pPixelShader->Release(); }
//    if (m_pBackBufferRTView) { m_pBackBufferRTView->Release(); }
//    if (m_pSwapChain) { m_pSwapChain->Release(); }
//    if (m_pImmediateContext) { m_pImmediateContext->Release(); }
//    if (m_model) { delete m_model; }
//    if (m_pD3DDevice) { m_pD3DDevice->Release(); }
//}
//#pragma endregion