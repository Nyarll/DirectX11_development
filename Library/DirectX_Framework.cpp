#include "DirectX_Framework.h"

// 終了時解放処理
static VOID Cleanup();
// メッセージプロージャ
static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ID3D11Device* Device = NULL;
ID3D11DeviceContext* DeviceContext = NULL;
IDXGISwapChain* SwapChain = NULL;
ID3D11RenderTargetView* RenderTargetView = NULL;

ID3D11Texture2D*	DepthStencil;
ID3D11DepthStencilView* DepthStencilView = NULL;

UINT			FeatureLevels = 3;	  // 配列の要素数
D3D_FEATURE_LEVEL	FeatureLevel; // デバイス作成時に返される機能レベル


HWND CreateWindowHandle()
{
	HWND hWnd = NULL;

	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"Direct3D Tutorial", NULL };

	RegisterClassEx(&wc);

	// Create the application's window.
	hWnd = CreateWindow(L"Direct3D Tutorial", WINDOW_NAME,
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
	//レンダーターゲットビューの作成
	ID3D11Texture2D *BackBuffer;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBuffer);
	Device->CreateRenderTargetView(BackBuffer, NULL, &RenderTargetView);
	BackBuffer->Release();
	DeviceContext->OMSetRenderTargets(1, &RenderTargetView, NULL);

	//ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	DeviceContext->RSSetViewports(1, &vp);

	// 深度/ステンシル・テクスチャの作成
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
