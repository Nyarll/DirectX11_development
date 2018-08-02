#include "Game.h"


Game::Game(HWND hWnd)
{
	pos = { SCREEN_WIDTH/2 , SCREEN_HEIGHT/2  };
	vel = { 1,1 };
}

Game::~Game()
{

}

void Game::Run()
{
	if (pos.x <= SCREEN_LEFT || pos.x >= SCREEN_RIGHT)
	{
		vel.x *= -1.0;
	}
	if (pos.y <= SCREEN_TOP || pos.y >= SCREEN_BOTTOM)
	{
		vel.y *= -1.0;
	}

	pos.x += vel.x;
	pos.y += vel.y;
}

void Game::Draw()
{
	DrawTriangle(GetVector2D(30,30), GetVector2D(30, 180), GetVector2D(180, 180), COLOR_RED);
	DrawBox(GetVector2D(pos.x*2 - 50, pos.y*2 - 50), GetVector2D(pos.x*2 + 50, pos.y*2 + 50), COLOR_YELLOW);
	//DrawBox(GetVector2D(500, 300), GetVector2D(600, 400), COLOR_BLUE);
	//DrawCircle(pos, 300, COLOR_BLUE);
}
