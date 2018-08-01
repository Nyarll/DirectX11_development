#include "DirectX_Framework.h"
#include "vs.h"
#include "pixel.h"

// 終了時解放処理
static VOID Cleanup();
// メッセージプロージャ
static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


Vertex_t TriangleList[]{
	{ { -0.5f,  0.5f, 0.5f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
	{ { 0.5f, -0.5f, 0.5f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
	{ { -0.5f, -0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f, 1.0f } }
};

Vertex_t SquareList[]{
	{ { -0.5f,  0.5f, 0.5f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
	{ { 0.5f, -0.5f, 0.5f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
	{ { -1.0f, -0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
	{ { 1.0f,  0.5f, 0.5f },{ 1.0f, 1.0f, 0.0f, 1.0f } }
};



ID3D11Device* Device = NULL;
ID3D11DeviceContext* DeviceContext = NULL;
IDXGISwapChain* SwapChain = NULL;
ID3D11RenderTargetView* RenderTargetView = NULL;
ID3D11Texture2D*	DepthStencil;
ID3D11DepthStencilView* DepthStencilView = NULL;
UINT			FeatureLevels = 3;	  // 配列の要素数
D3D_FEATURE_LEVEL	FeatureLevel; // デバイス作成時に返される機能レベル
D3D11_VIEWPORT ViewPort;
ID3D11Buffer*           VertexBuffer;
ID3D11Buffer*			IndexBuffer;
ID3D11InputLayout*      InputLayout;
ID3D11VertexShader*     VertexShader;
ID3D11PixelShader*      PixelShader;

D3D11_INPUT_ELEMENT_DESC VertexDesc[]{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
WORD IndexList[]{
	0, 1, 2,
	0, 3, 1,
};

HWND CreateWindowHandle()
{
	HWND hWnd = NULL;

	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"Direct3D Tutorial", NULL };

	RegisterClassEx(&wc);

	// Create the application's window.
	hWnd = CreateWindow("Direct3D Tutorial", WINDOW_NAME,
		WS_OVERLAPPEDWINDOW, (700 - (WINDOW_WIDTH / 2)), (200 - WINDOW_HEIGHT / 5),
		WINDOW_WIDTH, WINDOW_HEIGHT,
		GetDesktopWindow(), NULL, wc.hInstance, NULL);


	return hWnd;
}

// Direct3Dの初期化関数
HRESULT InitDirectX(HWND hWnd)
{
	HRESULT hr = S_OK;



	// デバイスとスワップチェーンの作成
	{
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;         //バックバッファの数
		sd.BufferDesc.Width = WINDOW_WIDTH;     //バッファの幅
		sd.BufferDesc.Height = WINDOW_HEIGHT;    //バッファの高さ
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //バッファのフォーマット
		sd.BufferDesc.RefreshRate.Numerator = 60;   //リフレッシュレート
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // モード自動切り替え

		FeatureLevel = D3D_FEATURE_LEVEL_11_0;

		hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_REFERENCE, NULL, 0,
			&FeatureLevel, 1,
			D3D11_SDK_VERSION, &sd, &SwapChain, &Device, NULL, &DeviceContext);
		if (FAILED(hr))
		{
			hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_WARP, NULL, 0, &FeatureLevel, 1, D3D11_SDK_VERSION, &sd, &SwapChain, &Device, NULL, &DeviceContext);
			if (FAILED(hr))
			{
				hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_REFERENCE, NULL, 0, &FeatureLevel, 1, D3D11_SDK_VERSION, &sd, &SwapChain, &Device, NULL, &DeviceContext);
				if (FAILED(hr))
				{
					return E_FAIL;
				}
			}
		}
	}

	//ビューポートの設定
	{
		ViewPort.Width = WINDOW_WIDTH;
		ViewPort.Height = WINDOW_HEIGHT;
		ViewPort.MinDepth = 0.0f;
		ViewPort.MaxDepth = 1.0f;
		ViewPort.TopLeftX = 0;
		ViewPort.TopLeftY = 0;
		DeviceContext->RSSetViewports(1, &ViewPort);


	}

	// 深度/ステンシル・テクスチャの作成
	{
		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = WINDOW_WIDTH;     // 幅
		descDepth.Height = WINDOW_HEIGHT;	// 高さ
		descDepth.MipLevels = 1;		// ミップマップ レベル数
		descDepth.ArraySize = 1;		// 配列サイズ
		descDepth.Format = DXGI_FORMAT_D32_FLOAT; // フォーマット(深度のみ)
		descDepth.SampleDesc.Count = 1;    // マルチサンプリングの設定
		descDepth.SampleDesc.Quality = 0;    // マルチサンプリングの品質
		descDepth.Usage = D3D11_USAGE_DEFAULT;		// 使用方法 デフォルト
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;	// 深度/ステンシルとして使用
		descDepth.CPUAccessFlags = 0;    // CPUからアクセスしない
		descDepth.MiscFlags = 0;    // その他の設定なし
		hr = Device->CreateTexture2D(
			&descDepth,		// 作成する2Dテクスチャ
			NULL,			// 
			&DepthStencil	// 作成したテクスチャを受け取る
		);
		if (FAILED(hr))
		{
			return E_FAIL;
		}


		// 深度/ステンシルビューの作成
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Flags = 0;
		descDSV.Texture2D.MipSlice = 0;
		hr = Device->CreateDepthStencilView(
			DepthStencil,	// 深度/ステンシルビューを作るテクスチャ
			&descDSV,		// 深度/ステンシルビューの設定
			&DepthStencilView	// 作成したビューを受け取る
		);
		if (FAILED(hr))
		{
			return E_FAIL;
		}
	}

	//レンダーターゲットビューの作成
	ID3D11Texture2D *BackBuffer;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBuffer);
	Device->CreateRenderTargetView(BackBuffer, NULL, &RenderTargetView);
	BackBuffer->Release();
	DeviceContext->OMSetRenderTargets(1, &RenderTargetView, NULL);



	return S_OK;
}

void ScreenClear()
{
	// 描画ターゲットのクリア
	float clearColor[4] = { 0, 0, 0, 0 }; // クリアする値
	DeviceContext->ClearRenderTargetView(
		RenderTargetView,	// クリアする描画ターゲット
		clearColor		// クリアする値
	);

	// 深度/ステンシル値のクリア
	DeviceContext->ClearDepthStencilView(
		DepthStencilView,	// クリアする深度/ステンシルビュー
		D3D11_CLEAR_DEPTH,	// 深度値だけクリアする
		1.0f,			// 深度バッファをクリアする値
		0			// ステンシルバッファをクリアする値(今回は関係なし)
	);
}
// 描画
HRESULT ScreenFlip()
{
	// バックバッファの表示
	HRESULT hr = SwapChain->Present(
		0,	// 画面をすぐに更新する
		0	// 画面を実際に更新する
	);

	return hr;
}

//終了時解放処理
static VOID Cleanup()
{
	SwapChain->Release();
	RenderTargetView->Release();
	DeviceContext->Release();
	Device->Release();
}

//メッセージプロシージャ
static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY://終了時
		Cleanup();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


// 描画系

// red, green, blue の値からカラーコードをゲットする
UINT GetColor(UINT red, UINT green, UINT blue)
{
	UINT color_buf = 0;

	color_buf = color_buf | red;	// 赤色成分のカラーコードをorする
	color_buf = color_buf << 8;		// 8bitズラす
	color_buf = color_buf | green;	// 緑色成分のカラーコードをorする
	color_buf = color_buf << 8;		// 8bitズラす
	color_buf = color_buf | blue;	// 青色成分のカラーコードをorする
	color_buf = color_buf | 0xFF000000;	// alpha 要素も一応

	return color_buf;
}
// カラーコードをred,green,blueに変換する
Color_t ColorConversion(UINT color_code)
{
	unsigned int colorbuffer = color_code & 0x00FFFFFF;
	unsigned int buf = 0;
	float red = 0.0f, green = 0.0f, blue = 0.0f;

	buf = (colorbuffer & 0x00FF0000);
	buf = buf >> 16;
	red = ((float)(buf)) / 255;

	buf = (colorbuffer & 0x0000FF00);
	buf = buf >> 8;
	green = ((float)(buf)) / 255;

	buf = (colorbuffer & 0x000000FF);
	blue = ((float)(buf)) / 255;

	return{ red,green,blue };
}

// (pos1, pos2, pos3)
void DrawTriangle()
{
	HRESULT hr = S_OK;
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f }; //red,green,blue,alpha

	Vertex_t TriangleList[]{
		{ { -0.5f,  0.5f, 0.5f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
		{ { 0.5f, -0.5f, 0.5f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { -0.5f, -0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f, 1.0f } }
	};

	//頂点バッファ作成
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(Vertex_t) * 3;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subResourceData;
	subResourceData.pSysMem = TriangleList;
	subResourceData.SysMemPitch = 0;
	subResourceData.SysMemSlicePitch = 0;

	hr = Device->CreateBuffer(&bufferDesc, &subResourceData, &VertexBuffer);
	if (FAILED(hr))
		return;

	//頂点レイアウト作成
	hr = Device->CreateInputLayout(VertexDesc, ARRAYSIZE(VertexDesc),
		g_vs_main, sizeof(g_vs_main), &InputLayout);
	if (FAILED(hr))
		return;

	//頂点シェーダー生成
	hr = Device->CreateVertexShader(&g_vs_main, sizeof(g_vs_main), NULL, &VertexShader);
	if (FAILED(hr))
		return;

	//ピクセルシェーダー生成
	hr = Device->CreatePixelShader(&g_ps_main, sizeof(g_ps_main), NULL, &PixelShader);
	if (FAILED(hr))
		return;

	//
	UINT strides = sizeof(Vertex_t);
	UINT offsets = 0;
	DeviceContext->IASetInputLayout(InputLayout);
	DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &strides, &offsets);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DeviceContext->VSSetShader(VertexShader, NULL, 0);
	DeviceContext->RSSetViewports(1, &ViewPort);
	DeviceContext->PSSetShader(PixelShader, NULL, 0);
	DeviceContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);

	DeviceContext->ClearRenderTargetView(RenderTargetView, clearColor);
	DeviceContext->ClearDepthStencilView(DepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	DeviceContext->Draw(4, 0);

	SwapChain->Present(0, 0);
}

// (Left Up Pos, Right Down Pos, ColorCode)
void DrawBox(Vector2D pos1, Vector2D pos2, UINT color_code)
{
	HRESULT hr = S_OK;
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f }; //red,green,blue,alpha

	Color_t color = ColorConversion(color_code);

	Vertex_t SquareList[] = {
		/* 左上 */{ { (pos1.x / SCREEN_WIDTH) - 1.0f,  -(pos1.y / SCREEN_HEIGHT) + 1.0f, 0.5f },{ color.red, color.green, color.blue, 1.0f} },
		/* 右下 */{ { (pos2.x / SCREEN_WIDTH) - 1.0f, -(pos2.y / SCREEN_HEIGHT) + 1.0f, 0.5f },{ color.red, color.green, color.blue, 1.0f } },
		/* 左下 */{ { (pos1.x / SCREEN_WIDTH) - 1.0f, -(pos2.y / SCREEN_HEIGHT) + 1.0f, 0.5f },{ color.red, color.green, color.blue, 1.0f } },
		/* 右上 */{ { (pos2.x / SCREEN_WIDTH) - 1.0f,  -(pos1.y / SCREEN_HEIGHT) + 1.0f, 0.5f },{ color.red, color.green, color.blue, 1.0f } }
	};

	D3D11_BUFFER_DESC vbDesc;
	vbDesc.ByteWidth = sizeof(Vertex_t) * 4;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vrData;
	vrData.pSysMem = SquareList;
	vrData.SysMemPitch = 0;
	vrData.SysMemSlicePitch = 0;

	D3D11_BUFFER_DESC ibDesc;
	ibDesc.ByteWidth = sizeof(WORD) * 6;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA irData;
	irData.pSysMem = IndexList;
	irData.SysMemPitch = 0;
	irData.SysMemSlicePitch = 0;

	hr = Device->CreateBuffer(&ibDesc, &irData, &IndexBuffer);
	if (FAILED(hr))
		return;

	hr = Device->CreateBuffer(&vbDesc, &vrData, &VertexBuffer);
	if (FAILED(hr))
		return;

	//頂点レイアウト作成
	hr = Device->CreateInputLayout(VertexDesc, ARRAYSIZE(VertexDesc),
		g_vs_main, sizeof(g_vs_main), &InputLayout);
	if (FAILED(hr))
		return;

	//頂点シェーダー生成
	hr = Device->CreateVertexShader(&g_vs_main, sizeof(g_vs_main), NULL, &VertexShader);
	if (FAILED(hr))
		return;

	//ピクセルシェーダー生成
	hr = Device->CreatePixelShader(&g_ps_main, sizeof(g_ps_main), NULL, &PixelShader);
	if (FAILED(hr))
		return;


	// ここから下はテンプレ
	UINT strides = sizeof(Vertex_t);
	UINT offsets = 0;
	//D3D11Trans
	DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &strides, &offsets);
	DeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DeviceContext->IASetInputLayout(InputLayout);

	DeviceContext->VSSetShader(VertexShader, NULL, 0);
	DeviceContext->RSSetViewports(1, &ViewPort);
	DeviceContext->PSSetShader(PixelShader, NULL, 0);
	DeviceContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);

	DeviceContext->ClearRenderTargetView(RenderTargetView, clearColor);
	DeviceContext->ClearDepthStencilView(DepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	DeviceContext->DrawIndexed(6, 0, 0);

	SwapChain->Present(0, 0);
}