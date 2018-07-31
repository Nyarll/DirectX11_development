#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Include
#include <d3d11.h>
#include <Windows.h>

// Define
#define WINDOW_NAME L"Test Window"

#define WINDOW_WIDTH (640)		// Window ‚Ì•
#define WINDOW_HEIGHT (480)		// Window ‚Ì‚‚³


// WindowHandle ‚Ìì¬
HWND CreateWindowHandle();

void ScreenClear();
HRESULT ScreenFlip();

// Direct3D‚Ì‰Šú‰»ŠÖ”
HRESULT InitDirectX(HWND hWnd);
