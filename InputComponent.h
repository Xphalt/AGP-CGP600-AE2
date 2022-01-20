#pragma once

class GameObject;
class InputKeyboard;

class InputComponent
{
public:
	InputComponent(GameObject* _gameObject, InputKeyboard* _inputKeyboard);
	~InputComponent();

	void Update(GameObject& _gameObject);

private:
	GameObject* m_gameObject{ nullptr };
	InputKeyboard* m_InputKeyboard{ nullptr };
};