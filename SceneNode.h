#pragma once
#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

class Model;

class SceneNode
{
public:
	SceneNode();
	~SceneNode();

	void AddChildNode(SceneNode* _node);
	bool DetachNode(SceneNode* _node);
	void Execute(XMMATRIX* _world, XMMATRIX* _view, XMMATRIX* _projection);
	// Setters
	void SetModel(Model* _model);
	void SetXPos(float _xPos);
	void SetYPos(float _yPos);
	void SetZPos(float _zPos);
	void SetXAngle(float _xAngle);
	void SetYAngle(float _yAngle);
	void SetZAngle(float _zAngle);
	void SetScale(float _scale);

	// Getters
	const float GetXPos() { return m_x; }
	const float GetZPos() { return m_y; }
	const float GetYPos() { return m_z; }
	const float GetXAngle() { return m_xAngle; }
	const float GetYAngle() { return m_yAngle; }
	const float GetZAngle() { return m_zAngle; }
	const float GetScale() { return m_scale; }

private:
	Model* m_pModel{ nullptr };
	std::vector<SceneNode*> m_pChildren{ nullptr };

	float m_x, m_y, m_z;
	float m_xAngle, m_yAngle, m_zAngle;
	float m_scale;

};