#pragma once

class InputComponent;
class GraphicsComponent;
class PhysicsComponent;

class GameObject
{
public:
	GameObject(InputComponent* _input, GraphicsComponent* _graphics, PhysicsComponent* _physics);
	~GameObject();

	void Update();

private:
	InputComponent* m_input { nullptr };
	GraphicsComponent* m_graphics { nullptr };
	PhysicsComponent* m_physics { nullptr };
};