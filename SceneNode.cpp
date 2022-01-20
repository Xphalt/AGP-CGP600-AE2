#include "SceneNode.h"
#include "Model.h"

SceneNode::SceneNode()
	: m_pModel{nullptr}, m_x{0.0f}, m_y{0.0f}, m_z{0.0f},
	  m_xAngle{ 0.0f }, m_yAngle{ 0.0f }, m_zAngle{ 0.0f },
	  m_scale{ 1.0f}
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::AddChildNode(SceneNode* _node)
{
	m_pChildren.push_back(_node);
}

bool SceneNode::DetachNode(SceneNode* _node)
{
	for (int i = 0; i < m_pChildren.size(); i++)
	{
		if (_node == m_pChildren[i])
		{
			m_pChildren.erase(m_pChildren.begin() + 1);

			return true;
		}

		if (m_pChildren[i]->DetachNode(_node) == true) { return true; }
	}

	return false;
}

void SceneNode::Execute(XMMATRIX* _world, XMMATRIX* _view, XMMATRIX* _projection)
{
	XMMATRIX localWorld = XMMatrixIdentity();
	localWorld = XMMatrixRotationX(XMConvertToRadians(m_xAngle));
	localWorld *= XMMatrixRotationY(XMConvertToRadians(m_yAngle));
	localWorld *= XMMatrixRotationZ(XMConvertToRadians(m_zAngle));

	localWorld *= XMMatrixScaling(m_scale, m_scale, m_scale);
	localWorld *= XMMatrixTranslation(m_x, m_y, m_z);
	localWorld *= *_world;

	if (m_pModel) { m_pModel->Draw(_view, _projection); }

	for (int i = 0; i < m_pChildren.size(); i++)
	{
		m_pChildren[i]->Execute(&localWorld, _view, _projection);
	}
}

#pragma region Setters
void SceneNode::SetModel(Model* _model)
{
	m_pModel = _model;
}

void SceneNode::SetXPos(float _xPos)
{
	m_x = _xPos;
}

void SceneNode::SetYPos(float _yPos)
{
	m_y = _yPos;
}

void SceneNode::SetZPos(float _zPos)
{
	m_z = _zPos;
}

void SceneNode::SetXAngle(float _xAngle)
{
	m_xAngle = _xAngle;
}

void SceneNode::SetYAngle(float _yAngle)
{
	m_yAngle = _yAngle;
}

void SceneNode::SetZAngle(float _zAngle)
{
	m_zAngle = _zAngle;
}

void SceneNode::SetScale(float _scale)
{
	m_scale = _scale;
}
#pragma endregion