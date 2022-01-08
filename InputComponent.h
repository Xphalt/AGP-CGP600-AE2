#pragma once

class GameObject;

class InputComponent
{
private:

public:
	InputComponent();
	~InputComponent();

	void Update(GameObject& _gameObject);
};