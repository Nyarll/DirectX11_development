#pragma once

#include "../Library/DirectX_Framework.h"
#include <Windows.h>

class Game
{
public:
	Game(HWND hWnd);
	~Game();
	void Run();
	void Draw();
};

