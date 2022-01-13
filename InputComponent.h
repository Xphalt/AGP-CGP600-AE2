#pragma once

class GameObject;

class InputComponent
{

public:
	InputComponent();
	~InputComponent();

	void Update(GameObject& _gameObject);

private:
};