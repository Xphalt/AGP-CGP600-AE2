#include "InputComponent.h"
#include "ModuleKeyboard.h"

void InputComponent::Update(GameObject& _gameObject)
{
	m_pModuleKeyboard->Update(_gameObject);
}