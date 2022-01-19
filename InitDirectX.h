#pragma once
#include "Renderer.h"

class InitDirectX
{
public:
	InitDirectX(const InitDirectX&) = delete;
	static InitDirectX& GetInstance() { return s_instance; }

	HRESULT Initialise();
	D3D_DRIVER_TYPE* GetDriverType() { return &m_driverType; }
	D3D_FEATURE_LEVEL* GetFeatureLevel() { return &m_featureLevel; }

private:
	InitDirectX() {};

	HRESULT InitialiseDirectX(HRESULT& hr);
	const HRESULT& InitSwapChain(const UINT& width, const UINT& height, const UINT& numDriverTypes, D3D_DRIVER_TYPE  driverTypes[3], HRESULT& hr, const UINT& createDeviceFlags, D3D_FEATURE_LEVEL  featureLevels[3], const UINT& numFeatureLevels);
	const HRESULT& InitBackBuffer(HRESULT& hr, ID3D11Texture2D*& pBackBuffer);
	const HRESULT& InitRenderTargetView(HRESULT& hr, ID3D11Texture2D* pBackBuffer);
	const HRESULT& InitDepthStencil(const UINT& width, const UINT& height, HRESULT& hr);
	void InitViewport(const UINT& width, const UINT& height);
	const HRESULT& InitRasteriserState(HRESULT& hr);
	const HRESULT& InitSamplerState(HRESULT& hr);
	void InitialiseShaders();
	HRESULT InitialiseScene(HRESULT& hr);

	static InitDirectX s_instance;
	D3D_DRIVER_TYPE m_driverType { D3D_DRIVER_TYPE_NULL };
	D3D_FEATURE_LEVEL m_featureLevel { D3D_FEATURE_LEVEL_11_0 };
};