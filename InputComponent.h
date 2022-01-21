#pragma once

class GameObject;
class ModuleKeyboard;

class InputComponent
{
public:
	void Update(GameObject& _gameObject);

private:
	ModuleKeyboard* m_pModuleKeyboard{ nullptr };
};