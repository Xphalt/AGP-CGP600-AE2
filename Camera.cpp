#pragma once
#include "Camera.h"

Camera::Camera(float _initialXPos, float _initialYPos, float _intitialZPos, float _initialRotation)
	: m_x {_initialXPos}, m_y {_initialYPos}, m_z {_intitialZPos}, m_camera_rotation {_initialRotation}
{
	m_dx = sin(m_camera_rotation * (XM_PI / 180.0));
	m_dz = cos(m_camera_rotation * (XM_PI / 180.0));
}

void Camera::Rotate(float _rotation)
{
	m_camera_rotation = _rotation;
	m_dx = sin(m_camera_rotation * (XM_PI / 180.0));
	m_dz = cos(m_camera_rotation * (XM_PI / 180.0));
}

void Camera::Forward(float _distance)
{
	m_x = m_dx * _distance;
	m_z = m_dz * _distance;
}

void Camera::Up(float _distance)
{
	m_y = m_dy * _distance;
}

XMMATRIX Camera::GetViewMatrix()
{
	XMVECTOR position = XMVectorSet(0.0, 0.0, -5.0, 0.0);
	XMVECTOR lookat = XMVectorSet(0.0, 0.0, -4.0, 0.0);
	XMVECTOR up = XMVectorSet(0.0, 1.0, 0.0, 0.0);

	XMMATRIX view = XMMatrixLookAtLH(position, lookat, up);

	return view;
}