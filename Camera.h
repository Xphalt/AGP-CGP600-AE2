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
	float m_camera_rotation;
	XMVECTOR m_position, m_lookat, m_up;

public:
	Camera(float _initialXPos, float _initialYPos, float _intitialZPos, float _initialCameraRotation);

	void Rotate(float _rotation);
	void Forward(float _distance);
	void Up(float _distance);
	XMMATRIX GetViewMatrix();
};