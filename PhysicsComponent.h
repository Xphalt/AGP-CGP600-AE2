#pragma once

class GameObject;

class PhysicsComponent
{
public:
	PhysicsComponent();
	~PhysicsComponent();

	void Update(GameObject& _gameObject);

private:

};