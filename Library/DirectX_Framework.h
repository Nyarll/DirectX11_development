#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Include
#include "Vector.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include <Windows.h>

using namespace DirectX;

// Define
#define WINDOW_NAME "Test Window"

#define WINDOW_WIDTH (640)		// Window の幅
#define WINDOW_HEIGHT (480)		// Window の高さ
#define SCREEN_WIDTH (WINDOW_WIDTH)
#define SCREEN_HEIGHT (WINDOW_HEIGHT)

typedef struct Vertex_s
{
	float pos[3];
	float col[4];
}Vertex_t;

typedef struct Color_s
{
	float red, green, blue;
}Color_t;

enum ColorCode : unsigned int
{
	COLOR_BLACK = 0xFF000000,    // 黒色
	COLOR_NAVY = 0xFF000080,    // 濃紺色
	COLOR_GREEN = 0xFF008000,    // 緑色
	COLOR_TEAL = 0xFF008080,    // 青緑色
	COLOR_MAROON = 0xFF800000,    // マルーン色
	COLOR_PURPLE = 0xFF800080,    // 紫色
	COLOR_OLIVE = 0xFF808000,    // オリーブ色
	COLOR_GRAY = 0xFF808080,    // 灰色
	COLOR_SILVER = 0xFFC0C0C0,    // 銀色
	COLOR_BLUE = 0xFF0000FF,    // 青色
	COLOR_LIME = 0xFF00FF00,    // ライムグリーン色
	COLOR_AQUA = 0xFF00FFFF,    // 水色
	COLOR_RED = 0xFFFF0000,    // 赤色
	COLOR_FUCHSIA = 0xFFFF00FF,    // 赤紫色
	COLOR_YELLOW = 0xFFFFFF00,    // 黄色
	COLOR_WHITE = 0xFFFFFFFF,    // 白色
};

// WindowHandle の作成
HWND CreateWindowHandle();

void ScreenClear();
HRESULT ScreenFlip();

// Direct3Dの初期化関数
HRESULT InitDirectX(HWND hWnd);

// 描画系

UINT GetColor(UINT red, UINT green, UINT blue);
Color_t ColorConversion(UINT color_code);

void DrawTriangle();
void DrawBox(Vector2D pos1, Vector2D pos2, UINT color_code);