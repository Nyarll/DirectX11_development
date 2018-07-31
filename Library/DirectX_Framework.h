#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Include
#include <d3d11.h>
#include <Windows.h>

// Define
#define WINDOW_NAME L"Test Window"

#define WINDOW_WIDTH (640)		// Window の幅
#define WINDOW_HEIGHT (480)		// Window の高さ

// WindowHandle の作成
HWND CreateWindowHandle();

void DrawTriangle();

void ScreenClear();
HRESULT ScreenFlip();

// Direct3Dの初期化関数
HRESULT InitDirectX(HWND hWnd);

ID3D11Device* Get3DDevice();