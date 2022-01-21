#include "GameObject.h"
#include "InputComponent.h"

void GameObject::Update(InputComponent* _input)
{
	_input->Update(*this);
}