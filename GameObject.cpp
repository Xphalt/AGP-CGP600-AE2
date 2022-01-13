#pragma once
#include "GameObject.h"
#include "InputComponent.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"

GameObject::GameObject(InputComponent* _input, GraphicsComponent* _graphics, PhysicsComponent* _physics)
	: m_input {_input}, m_graphics {_graphics}, m_physics {_physics}
{
}

GameObject::~GameObject()
{

    if (nullptr != m_physics)
    {
        delete m_physics;
        m_physics = nullptr;
    }

    if (nullptr != m_graphics)
    {
        delete m_graphics;
        m_graphics = nullptr;
    }

    if (nullptr != m_input)
    {
        delete m_input;
        m_input = nullptr;
    }
}

void GameObject::Update()
{
	m_input->Update(*this);
	m_graphics->Update(*this);
	m_physics->Update(*this);
}