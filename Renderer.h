#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <d3dX11.h>
#include <windows.h>
#include <dxerr.h>
#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <DirectXMath.h>

using namespace DirectX;

#include "Shaders.h"
#include "Vertex.h"

class Shaders;
struct Vertex;

class Renderer
{
public:
	friend class InitDirectX;
	Renderer(const Renderer&) = delete;
	~Renderer();
	static Renderer&		GetInstance()		    { return s_instance; };

	void					  RenderFrame(void);	    
	HWND&					  GetHWND()			     { return m_hwnd;}
	ID3D11Device*			  GetDevice()		     { return m_pDevice; }
	ID3D11DeviceContext*	  GetDeviceContext()	 { return m_pDeviceContext; }
	IDXGISwapChain*			  GetSwapChain()		 { return m_pSwapChain; }
	ID3D11RenderTargetView*	  GetRenderTargetView()  { return m_pRenderTargetView; }
	Shaders*				  GetShaders()		     { return m_pShaders; }
	ID3D11Buffer*			  GetVertexBuffer()	     { return m_pVertexBuffer; }
	ID3D11RasterizerState*	  GetRasterState()		 { return m_pRasterState; }
	ID3D11DepthStencilView*   GetDSV()				 { return m_pDSV; }
	ID3D11Texture2D*		  GetDSB()				 { return m_pDSB; }
	ID3D11DepthStencilState*  GetDSS()				 { return m_pDSS; }
	ID3D11SamplerState*		  GetSamplerState()		 { return m_pSamplerState; }
	ID3D11ShaderResourceView* GetTexture()			 { return m_pTexture; }
private:										    
	static Renderer			 s_instance;		    
	Renderer();								    
												    
	HWND					  m_hwnd				    { nullptr };
	ID3D11Device*		      m_pDevice					{ nullptr };
	ID3D11DeviceContext*	  m_pDeviceContext			{ nullptr };
	IDXGISwapChain*			  m_pSwapChain				{ nullptr };
	ID3D11RenderTargetView*   m_pRenderTargetView		{ nullptr };
	Shaders*				  m_pShaders			    { nullptr };
	ID3D11Buffer*			  m_pVertexBuffer			{ nullptr };
	ID3D11Buffer*			  m_pIndicesBuffer			{ nullptr };
	ID3D11RasterizerState*	  m_pRasterState			{ nullptr };
	ID3D11DepthStencilView*   m_pDSV					{ nullptr };
	ID3D11Texture2D*		  m_pDSB					{ nullptr };
	ID3D11DepthStencilState*  m_pDSS					{ nullptr };
	ID3D11SamplerState*		  m_pSamplerState			{ nullptr };
	ID3D11ShaderResourceView* m_pTexture				{ nullptr };
};