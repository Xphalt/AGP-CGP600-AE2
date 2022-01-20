#include "InputComponent.h"

InputComponent::InputComponent(GameObject* _gameObject, InputKeyboard* _inputKeyboard)
	: m_gameObject{_gameObject}, m_InputKeyboard{_inputKeyboard}
{
}

InputComponent::~InputComponent()
{
}

void InputComponent::Update(GameObject& _gameObject)
{
}