#pragma once

#include "../Library/DirectX_Framework.h"
#include <Windows.h>

class Game
{
private:
	Vector2D pos;
	Vector2D vel;

public:
	Game(HWND hWnd);
	~Game();
	void Run();
	void Draw();
};

