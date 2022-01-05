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
	m_dx = XMConvertToRadians(sin(m_camera_rotation));
	m_dz = XMConvertToRadians(cos(m_camera_rotation));
}

void Camera::Forward(float _distance)
{
	m_x += m_dx * _distance;
	m_z += m_dz * _distance;
}

void Camera::Up(float _distance)
{
	m_y += _distance;
}

XMMATRIX Camera::GetViewMatrix()
{
	XMVECTOR position = XMVectorSet(m_x, m_y, m_z, 0.0);
	XMVECTOR lookat = XMVectorSet(m_x + m_dx, m_y, m_z + m_dz, 0.0);
	XMVECTOR up = XMVectorSet(0.0, 1.0, 0.0, 0.0);

	XMMATRIX view = XMMatrixLookAtLH(position, lookat, up);

	return view;
}

void Camera::Strafe(float _distance)
{
	/* Calculate forward vector, which is calculated from
	2 normalised points we know: lookat minus position */
	XMVECTOR forward = XMVector3Normalize(m_lookat - m_position);

	/* Calculate right vector, which is a normalised
	perpendicular vector to forward and up */
	XMVECTOR right = XMVector3Normalize(XMVector3Cross(forward, m_up));

	/* Update m_xand m_z, by multiplying right xand z by
	distance */
	m_x += m_dx * _distance;
	m_z += m_dz * _distance;
}