#include "Game.h"
#include "../Library/DirectX_Framework.h"


Game::Game(HWND hWnd)
{

}

Game::~Game()
{

}

void Game::Run()
{

}

void Game::Draw()
{
	//DrawTriangle();
	DrawBox(GetVector2D(150, 150), GetVector2D(400, 400), COLOR_YELLOW);
}
