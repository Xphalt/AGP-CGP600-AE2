#pragma once

class InputComponent;
class GraphicsComponent;
class PhysicsComponent;

class GameObject
{
private:
	GameObject* m_gameObject { nullptr };
	InputComponent* m_input { nullptr };
	GraphicsComponent* m_graphics { nullptr };
	PhysicsComponent* m_physics { nullptr };

public:
	GameObject(InputComponent* _input, GraphicsComponent* _graphics, PhysicsComponent* _physics);
	~GameObject();

	void Update();
};