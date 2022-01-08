#pragma once

class Model;
class Shader;
class Texture;
class Lighting;
class Renderer;
class GameObject;

class GraphicsComponent
{
private:
	Model* m_model { nullptr };

public:
	GraphicsComponent(Model* _model);
	~GraphicsComponent();

	void Update(GameObject& _gameObject);
};