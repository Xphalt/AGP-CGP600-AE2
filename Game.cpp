#pragma once
#include "Game.h"

void Game::Update()
{
	Renderer::GetInstance().RenderFrame();
}