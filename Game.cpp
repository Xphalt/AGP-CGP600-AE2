#pragma once
#include "Game.h"

Game::Game()
{
	m_rInitDirectX.Initialise(m_hwnd);
}

Game::~Game()
{
	
}

void Game::Update()
{
	m_rRenderer.RenderFrame();
}