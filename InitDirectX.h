#pragma once
#include "Renderer.h"
#include "Shaders.h"
#include "Vertex.h"

class Renderer;
class Shaders;

class InitDirectX
{
public:
	InitDirectX(const InitDirectX&) = delete;

	static InitDirectX&		GetInstance()			  { return s_instance; }
	HRESULT					Initialise(HWND _hwnd);
	D3D_DRIVER_TYPE			GetDriverType()		const { return m_driverType; }
	D3D_FEATURE_LEVEL		GetFeatureLevel()		const { return m_featureLevel; }

private:
	InitDirectX() {};
	void					InitialiseVertexBuffer(HRESULT& hr);
	void					InitialiseShaders();
	HRESULT					InitialiseDirectX(const HWND& _hwnd, HRESULT& hr);

	static InitDirectX		s_instance;
	Renderer&				m_rRenderer				  { Renderer::GetInstance() };
	D3D_DRIVER_TYPE			m_driverType			  { D3D_DRIVER_TYPE_NULL };
	D3D_FEATURE_LEVEL		m_featureLevel			  { D3D_FEATURE_LEVEL_11_0 };
};