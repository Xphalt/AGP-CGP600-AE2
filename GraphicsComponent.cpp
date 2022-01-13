#pragma once
#include "GraphicsComponent.h"
#include "InitDirectX.h"
#include "RenderWindow.h"
#include "Renderer.h"
#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "Lighting.h"
#include "Camera.h"
#include "text2D.h"

GraphicsComponent::GraphicsComponent(InitDirectX* _initDirectX, RenderWindow* _renderWindow)
	: m_pInitDirectX{_initDirectX}, m_pRenderWindow{_renderWindow}
{
}

GraphicsComponent::GraphicsComponent(Model* _model, Shader* _shader, Texture* _texture, Lighting* _lighting, Renderer* _renderer)
	: m_pModel{_model}, m_pShader{_shader}, m_pTexture{_texture}, m_pLighting{_lighting}, m_pRenderer{_renderer}
{
}

GraphicsComponent::~GraphicsComponent()
{
}

void GraphicsComponent::Update(GameObject& _gameObject)
{
	m_pModel->Update(_gameObject);
	m_pShader->Update(_gameObject);
	m_pTexture->Update(_gameObject);
	m_pLighting->Update(_gameObject);
	m_pRenderer->Update(_gameObject);
}