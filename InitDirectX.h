#pragma once
#include "Renderer.h"

class InitDirectX
{
public:
	InitDirectX(const InitDirectX&) = delete;
	static InitDirectX&		GetInstance()			  { return s_instance; }

	HRESULT					Initialise();
	D3D_DRIVER_TYPE*		GetDriverType()			  { return &m_driverType; }
	D3D_FEATURE_LEVEL*		GetFeatureLevel()		  { return &m_featureLevel; }

private:
	InitDirectX() {};

	HRESULT					InitialiseDirectX(HRESULT& hr);
	void					InitialiseShaders();
	HRESULT					InitialiseScene(HRESULT& hr);

	static InitDirectX		s_instance;
	D3D_DRIVER_TYPE			m_driverType			  { D3D_DRIVER_TYPE_NULL };
	D3D_FEATURE_LEVEL		m_featureLevel			  { D3D_FEATURE_LEVEL_11_0 };
};