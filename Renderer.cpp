#pragma once
#include "Renderer.h"
#include "GraphicsComponent.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Update(GameObject& _gameObject)
{
}

void Renderer::RenderFrame(void)
{
    float rgba_clear_colour[4] = { 0.1f, 0.2f, 0.6f, 1.0f };
    m_pImmediateContext->ClearRenderTargetView(m_pBackBufferRTView, rgba_clear_colour);
    m_pImmediateContext->ClearDepthStencilView(m_pZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    UINT stride = sizeof(POS_COL_TEX_NORM_VERTEX);
    UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pImmediateContext->VSSetShader(m_pVertexShader, 0, 0);
    m_pImmediateContext->PSSetShader(m_pPixelShader, 0, 0);
    m_pImmediateContext->IASetInputLayout(m_pInputLayout);

    m_directional_light_shines_from = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
    m_directional_light_colour = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    m_ambient_light_colour = XMVectorSet(0.1f, 0.1f, 0.1f, 1.0f);

    CONSTANT_BUFFER0 m_cb0Values;
    XMMATRIX view, world, projection, transpose;

    //view = XMMatrixIdentity();
    view = m_camera->GetViewMatrix();
    world = XMMatrixRotationX(XMConvertToRadians(60));
    world *= XMMatrixTranslation(-10, 0, 20);
    projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0), 640.0 / 480.0, 1.0, 100.0);
    transpose = XMMatrixTranspose(world);
    m_cb0Values.WorldViewProjection = world * view * projection;
    m_cb0Values.directional_light_colour = m_directional_light_colour;
    m_cb0Values.ambient_light_colour = m_ambient_light_colour;
    m_cb0Values.directional_light_vector = XMVector3Transform(m_directional_light_shines_from, transpose);
    m_cb0Values.directional_light_vector = XMVector3Normalize(m_cb0Values.directional_light_vector);

    m_pImmediateContext->UpdateSubresource(m_pConstantBuffer0, 0, 0, &m_cb0Values, 0, 0);
    m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer0);

    m_pImmediateContext->PSSetSamplers(0, 1, &m_pSampler0);
    m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTexture0);

    m_2DText->AddText("Some Text", -1.0, +1.0, .2);
    m_pImmediateContext->OMSetBlendState(m_pAlphaBlendEnable, 0, 0xffffffff);
    m_pImmediateContext->OMSetBlendState(m_pAlphaBlendDisable, 0, 0xffffffff);

    m_pImmediateContext->RSSetState(rastStateCullBack);
    m_model->Draw(&view, &projection);
    m_pImmediateContext->Draw(36, 0);
    m_pImmediateContext->RSSetState(rastStateCullNone);
    m_2DText->RenderText();
    m_pSwapChain->Present(0, 0);
}