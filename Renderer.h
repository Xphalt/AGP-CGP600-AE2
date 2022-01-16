#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <D3DX11.h>
#include <windows.h>
#include <dxerr.h>
#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <DirectXMath.h>

#include "Shaders.h"
#include "Vertex.h"

class Shaders;
class Vertex;

class Renderer
{
public:
	Renderer(const Renderer&) = delete;
	static Renderer&		GetInstance()				{ return s_instance; };
	
	void					RenderFrame(void);
	HWND					GetHWND()			   { return m_hwnd;}
	ID3D11Device*			GetDevice()			  const { return m_pDevice; }
	ID3D11DeviceContext*	GetDeviceContext()	  const { return m_pDeviceContext; }
	IDXGISwapChain*			GetSwapChain()		  const { return m_pSwapChain; }
	ID3D11RenderTargetView* GetRenderTargetView() const { return m_pRenderTargetView; }
	Shaders*				GetShaders()		  const { return m_pShaders; }
	ID3D11Buffer*			GetVertexBuffer()	  const { return m_pVertexBuffer; }

private:
	static Renderer			s_instance;
	Renderer() {};

	HWND					m_hwnd				  { NULL };
	ID3D11Device*		    m_pDevice			  { nullptr };
	ID3D11DeviceContext*	m_pDeviceContext	  { nullptr };
	IDXGISwapChain*			m_pSwapChain		  { nullptr };
	ID3D11RenderTargetView* m_pRenderTargetView	  { nullptr };
	Shaders*				m_pShaders			  { nullptr };
	ID3D11Buffer*			m_pVertexBuffer		  { nullptr };
};