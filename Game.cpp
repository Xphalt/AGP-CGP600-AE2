#pragma once
#include "Game.h"
#include "InitDirectX.h"
#include "RenderWindow.h"

Game::Game()
{
	m_pInitDirectX = new InitDirectX();
	m_pInitDirectX->Initialise(9, 9, m_hwnd);
	m_pRenderWindow = new RenderWindow(m_windowName, m_hwnd, m_hInstance, m_nCmdShow);
}

Game::~Game()
{
	if (m_pRenderWindow == nullptr) { delete m_pRenderWindow; m_pRenderWindow = { nullptr }; }
	if (m_pInitDirectX == nullptr) { delete m_pInitDirectX; m_pInitDirectX = { nullptr }; }
}

void Game::Update()
{
	
}