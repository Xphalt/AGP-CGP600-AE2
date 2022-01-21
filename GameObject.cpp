#include "GameObject.h"
#include "InputComponent.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"

void GameObject::Update(InputComponent* _input, GraphicsComponent* _graphics, PhysicsComponent* _physics)
{
	_input->Update(*this);
	_graphics->Update(*this);
	_physics->Update(*this);
}