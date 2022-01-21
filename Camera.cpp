#pragma once
#include "Camera.h"

Camera::Camera(float _initialXPos, float _initialYPos, float _intitialZPos, float _initialRotation)
	: m_x {_initialXPos}, m_y {_initialYPos}, m_z {_intitialZPos}, m_cameraRotation{_initialRotation}
{
	m_position = XMVectorSet(0.0, 0.0, -5.0, 0.0);
	m_lookat = XMVectorSet(0.0, 0.0, -4.0, 0.0);
	m_up = XMVectorSet(0.0, 1.0, 0, 0.0);
	m_dx = XMConvertToRadians(sin(m_cameraRotation));
	m_dz = XMConvertToRadians(cos(m_cameraRotation));
}

void Camera::Rotate(float _rotation)
{
	m_cameraRotation += _rotation;
	m_dx = XMConvertToRadians(sin(m_cameraRotation));
	m_dz = XMConvertToRadians(cos(m_cameraRotation));
}

void Camera::Forward(float _distance)
{
	m_x += _distance * m_dx;
	m_z += _distance * m_dz;
}

void Camera::Up(float _distance)
{
	m_y += _distance;
}

XMMATRIX Camera::GetViewMatrix()
{
	m_dx = sin(m_cameraRotation) * (XM_PI / 180.0);
	m_dz = cos(m_cameraRotation) * (XM_PI / 180.0);

	m_position = XMVectorSet(m_x, m_y, m_z, 0.0);
	m_lookat = XMVectorSet(m_x + m_dx, m_y, m_z + m_dz, 0.0);
	m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);

	return XMMatrixLookAtLH(m_position, m_lookat, m_up);
}

void Camera::Strafe(float _distance)
{
	XMVECTOR forward = XMVector3Normalize(m_lookat - m_position);
	XMVECTOR right = XMVector3Normalize(XMVector3Cross(forward, m_up));

	m_x += XMVectorGetX(right) * _distance;
	m_z += XMVectorGetZ(right) * _distance;
}