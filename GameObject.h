#pragma once

class InputComponent;
class GraphicsComponent;
class PhysicsComponent;

class GameObject
{
public:
	GameObject(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext);
	~GameObject();

	void Update(InputComponent* _input, GraphicsComponent* _graphics, PhysicsComponent* _physics);

	InputComponent* GetInputComponent() const { return m_pInputComponent; }
	GraphicsComponent* GetGraphicsComponent() const { return m_pGraphicsComponent; }
	PhysicsComponent* GetPhysicsComponent() const { return m_pPhysicsComponent; }

private:
	InputComponent* m_pInputComponent{ nullptr };
	GraphicsComponent* m_pGraphicsComponent{ nullptr };
	PhysicsComponent* m_pPhysicsComponent{ nullptr };
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
};