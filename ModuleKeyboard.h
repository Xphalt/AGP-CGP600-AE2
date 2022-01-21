#pragma once

class GameObject;
class InputKeyboard;

class ModuleKeyboard
{
public:
	virtual void Update(GameObject& _gameObject);

private:
	InputKeyboard* m_pInputKeyboard{ nullptr };
};