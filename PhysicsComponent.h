#pragma once

class GameObject;

class PhysicsComponent
{
private:

public:
	PhysicsComponent();
	~PhysicsComponent();

	void Update(GameObject& _gameObject);
};