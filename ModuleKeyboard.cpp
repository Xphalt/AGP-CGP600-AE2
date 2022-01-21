#include "ModuleKeyboard.h"
#include "InputKeyboard.h"

void ModuleKeyboard::Update(GameObject& _gameObject)
{
	m_pInputKeyboard->Update(_gameObject);
}