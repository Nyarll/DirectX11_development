#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Include
#include <d3d11.h>
#include <Windows.h>

// Define
#define WINDOW_NAME L"Test Window"

#define WINDOW_WIDTH (640)		// Window �̕�
#define WINDOW_HEIGHT (480)		// Window �̍���


// WindowHandle �̍쐬
HWND CreateWindowHandle();

void ScreenClear();
HRESULT ScreenFlip();

// Direct3D�̏������֐�
HRESULT InitDirectX(HWND hWnd);
