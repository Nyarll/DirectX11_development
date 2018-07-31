#include "Main/Game.h"
#include "Library/CreateWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	// WindowHandle の作成
	HWND hWnd = CreateWindowHandle();

	// ゲームオブジェクトの作成
	Game* game = new Game(hWnd);

	MSG msg;	ZeroMemory(&msg, sizeof(msg));	// メッセージループ用変数の宣言＆初期化

	
	if (SUCCEEDED(InitD3D(hWnd)))
	{
		// Window 表示
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				// ここにゲーム処理
				game->Run();
				ScreenClear();
				ScreenFlip();
			}
		}
	}

	delete game;
}