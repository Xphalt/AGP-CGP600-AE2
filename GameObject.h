#pragma once

class InputComponent;
class GraphicsComponent;
class PhysicsComponent;

class GameObject
{
public:
	void Update(InputComponent* _input, GraphicsComponent* _graphics, PhysicsComponent* _physics);

private:
	InputComponent* m_pInputComponent{ nullptr };
	GraphicsComponent* m_pGraphicsComponent{ nullptr };
	PhysicsComponent* m_pPhysicsComponent{ nullptr };
};