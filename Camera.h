#pragma once

#include <d3d11.h>
#include <math.h>
#define _XM_NO_INTRINSICS_
#define _XM_NO_ALIGNMENT_
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
private:
	float m_x, m_y, m_z;
	float m_dx, m_dy, m_dz;
	float m_cameraRotation;
	XMVECTOR m_position, m_lookat, m_up;
	float m_stepAmount;

public:
	Camera(float _initialXPos, float _initialYPos, float _intitialZPos, float _initialCameraRotation);

	void Rotate(float _rotation);
	void Forward(float _distance);
	void Up(float _distance);
	XMMATRIX GetViewMatrix();
	void Strafe(float _distance);

	void SetXPos(float _xPos);
	void SetYPos(float _yPos);
	void SetZPos(float _zPos);

	float GetXPos() const { return m_x; }
	float GetYPos() const { return m_y; }
	float GetZPos() const { return m_z; }
};