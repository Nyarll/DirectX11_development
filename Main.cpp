#include "Main/Game.h"
#include "Library/DirectX_Framework.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	// WindowHandle の作成
	HWND hWnd = CreateWindowHandle();

	// ゲームオブジェクトの作成
	Game* game = new Game(hWnd);

	MSG msg;	ZeroMemory(&msg, sizeof(msg));	// メッセージループ用変数の宣言＆初期化

	// DirectX の初期化が無事にできたら
	if (SUCCEEDED(InitDirectX(hWnd)))
	{
		// Window 表示
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
		
		// メッセージループ
		// ×ボタンをマウスカーソルでクリックすると終了する (デフォルト)
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
				ScreenClear();

				game->Run();
				game->Draw();

				ScreenFlip();
			}
		}
	}

	delete game;
	return 0;
}