#pragma once
#include "Game.h"

Game::Game()
{
	m_pInitDirectX = new InitDirectX();
	m_pRenderWindow = new RenderWindow(m_windowName, m_hwnd, m_hInstance, m_nCmdShow);
	m_pGraphics = new GraphicsComponent(m_pInitDirectX, m_pRenderWindow);

	m_pGraphics->GetInitDirectX()->Initialise(m_hwnd);
}

Game::~Game()
{
	if (m_pRenderWindow == nullptr) { delete m_pRenderWindow; m_pRenderWindow = { nullptr }; }
	if (m_pInitDirectX == nullptr) { delete m_pInitDirectX; m_pInitDirectX = { nullptr }; }
}

void Game::Update()
{
	
}