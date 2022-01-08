#pragma once
#include "GraphicsComponent.h"

GraphicsComponent::GraphicsComponent(Model* _model)
	: m_model {_model}
{
}

GraphicsComponent::~GraphicsComponent()
{
}

void GraphicsComponent::Update(GameObject& _gameObject)
{
}