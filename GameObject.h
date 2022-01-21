#pragma once

class InputComponent;

class GameObject
{
public:
	void Update(InputComponent* _input);

private:
	InputComponent* m_pInputComponent{ nullptr };
};