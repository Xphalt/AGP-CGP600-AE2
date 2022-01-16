#pragma once
#include "GraphicsComponent.h"
#include "InitDirectX.h"
#include "RenderWindow.h"
#include "Renderer.h"
#include "Model.h"
#include "Shaders.h"
#include "Texture.h"
#include "Lighting.h"
#include "Camera.h"
#include "text2D.h"

GraphicsComponent::GraphicsComponent(InitDirectX* _initDirectX, RenderWindow* _renderWindow)
	: m_pInitDirectX{_initDirectX}, m_pRenderWindow{_renderWindow}
{
}

GraphicsComponent::GraphicsComponent(Model* _model, Shaders* _shaders, Texture* _texture, Lighting* _lighting, Renderer* _renderer)
	: m_pModel{_model}, m_pShaders{_shaders}, m_pTexture{_texture}, m_pLighting{_lighting}, m_pRenderer{_renderer}
{
	
}

GraphicsComponent::~GraphicsComponent()
{
	if (m_pModel) { m_pModel = { nullptr }; }
	if (m_pModel) { m_pModel = { nullptr }; }
	if (m_pRenderer) { m_pRenderer = { nullptr }; }
	if (m_pLighting) { m_pLighting = { nullptr }; }
	if (m_pTexture) { m_pTexture = { nullptr }; }
	if (m_pShaders) { m_pShaders = { nullptr }; }
	if (m_pModel) { m_pModel = { nullptr }; }
	if (m_pRenderWindow) { m_pRenderWindow = { nullptr }; }
	if (m_pInitDirectX) { m_pInitDirectX = { nullptr }; }
}

void GraphicsComponent::Update(GameObject& _gameObject)
{
	m_pModel->Update(_gameObject);
	m_pTexture->Update(_gameObject);
	m_pLighting->Update(_gameObject);
}