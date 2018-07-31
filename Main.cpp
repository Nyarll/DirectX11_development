#include "Main/Game.h"
#include "Library/CreateWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	// WindowHandle �̍쐬
	HWND hWnd = CreateWindowHandle();

	// �Q�[���I�u�W�F�N�g�̍쐬
	Game* game = new Game(hWnd);

	MSG msg;	ZeroMemory(&msg, sizeof(msg));	// ���b�Z�[�W���[�v�p�ϐ��̐錾��������

	
	if (SUCCEEDED(InitD3D(hWnd)))
	{
		// Window �\��
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
				// �����ɃQ�[������
				game->Run();
				ScreenClear();
				ScreenFlip();
			}
		}
	}

	delete game;
}