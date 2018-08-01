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

#define WINDOW_WIDTH (640)		// Window �̕�
#define WINDOW_HEIGHT (480)		// Window �̍���
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
	COLOR_BLACK = 0xFF000000,    // ���F
	COLOR_NAVY = 0xFF000080,    // �Z���F
	COLOR_GREEN = 0xFF008000,    // �ΐF
	COLOR_TEAL = 0xFF008080,    // �ΐF
	COLOR_MAROON = 0xFF800000,    // �}���[���F
	COLOR_PURPLE = 0xFF800080,    // ���F
	COLOR_OLIVE = 0xFF808000,    // �I���[�u�F
	COLOR_GRAY = 0xFF808080,    // �D�F
	COLOR_SILVER = 0xFFC0C0C0,    // ��F
	COLOR_BLUE = 0xFF0000FF,    // �F
	COLOR_LIME = 0xFF00FF00,    // ���C���O���[���F
	COLOR_AQUA = 0xFF00FFFF,    // ���F
	COLOR_RED = 0xFFFF0000,    // �ԐF
	COLOR_FUCHSIA = 0xFFFF00FF,    // �Ԏ��F
	COLOR_YELLOW = 0xFFFFFF00,    // ���F
	COLOR_WHITE = 0xFFFFFFFF,    // ���F
};

// WindowHandle �̍쐬
HWND CreateWindowHandle();

void ScreenClear();
HRESULT ScreenFlip();

// Direct3D�̏������֐�
HRESULT InitDirectX(HWND hWnd);

// �`��n

UINT GetColor(UINT red, UINT green, UINT blue);
Color_t ColorConversion(UINT color_code);

void DrawTriangle();
void DrawBox(Vector2D pos1, Vector2D pos2, UINT color_code);