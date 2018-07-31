#include "DirectX_Framework.h"

// �I�����������
static VOID Cleanup();
// ���b�Z�[�W�v���[�W��
static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ID3D11Device* Device = NULL;
ID3D11DeviceContext* DeviceContext = NULL;
IDXGISwapChain* SwapChain = NULL;
ID3D11RenderTargetView* RenderTargetView = NULL;

ID3D11Texture2D*	DepthStencil;
ID3D11DepthStencilView* DepthStencilView = NULL;

UINT			FeatureLevels = 3;	  // �z��̗v�f��
D3D_FEATURE_LEVEL	FeatureLevel; // �f�o�C�X�쐬���ɕԂ����@�\���x��


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

// Direct3D�̏������֐�
HRESULT InitDirectX(HWND hWnd)
{
	HRESULT hr = S_OK;

	// �f�o�C�X�ƃX���b�v�`�F�[���̍쐬
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;         //�o�b�N�o�b�t�@�̐�
	sd.BufferDesc.Width = WINDOW_WIDTH;     //�o�b�t�@�̕�
	sd.BufferDesc.Height = WINDOW_HEIGHT;    //�o�b�t�@�̍���
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //�o�b�t�@�̃t�H�[�}�b�g
	sd.BufferDesc.RefreshRate.Numerator = 60;   //���t���b�V�����[�g
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // ���[�h�����؂�ւ�

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
	//�����_�[�^�[�Q�b�g�r���[�̍쐬
	ID3D11Texture2D *BackBuffer;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBuffer);
	Device->CreateRenderTargetView(BackBuffer, NULL, &RenderTargetView);
	BackBuffer->Release();
	DeviceContext->OMSetRenderTargets(1, &RenderTargetView, NULL);

	//�r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	DeviceContext->RSSetViewports(1, &vp);

	// �[�x/�X�e���V���E�e�N�X�`���̍쐬
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = WINDOW_WIDTH;     // ��
	descDepth.Height = WINDOW_HEIGHT;	// ����
	descDepth.MipLevels = 1;		// �~�b�v�}�b�v ���x����
	descDepth.ArraySize = 1;		// �z��T�C�Y
	descDepth.Format = DXGI_FORMAT_D32_FLOAT; // �t�H�[�}�b�g(�[�x�̂�)
	descDepth.SampleDesc.Count = 1;    // �}���`�T���v�����O�̐ݒ�
	descDepth.SampleDesc.Quality = 0;    // �}���`�T���v�����O�̕i��
	descDepth.Usage = D3D11_USAGE_DEFAULT;		// �g�p���@ �f�t�H���g
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;	// �[�x/�X�e���V���Ƃ��Ďg�p
	descDepth.CPUAccessFlags = 0;    // CPU����A�N�Z�X���Ȃ�
	descDepth.MiscFlags = 0;    // ���̑��̐ݒ�Ȃ�
	hr = Device->CreateTexture2D(
		&descDepth,		// �쐬����2D�e�N�X�`��
		NULL,			// 
		&DepthStencil	// �쐬�����e�N�X�`�����󂯎��
	);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// �[�x/�X�e���V���r���[�̍쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	hr = Device->CreateDepthStencilView(
		DepthStencil,	// �[�x/�X�e���V���r���[�����e�N�X�`��
		&descDSV,		// �[�x/�X�e���V���r���[�̐ݒ�
		&DepthStencilView	// �쐬�����r���[���󂯎��
	);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	return S_OK;
}

void ScreenClear()
{
	// �`��^�[�Q�b�g�̃N���A
	float clearColor[4] = { 0, 0, 0, 0 }; // �N���A����l
	DeviceContext->ClearRenderTargetView(
		RenderTargetView,	// �N���A����`��^�[�Q�b�g
		clearColor		// �N���A����l
	);

	// �[�x/�X�e���V���l�̃N���A
	DeviceContext->ClearDepthStencilView(
		DepthStencilView,	// �N���A����[�x/�X�e���V���r���[
		D3D11_CLEAR_DEPTH,	// �[�x�l�����N���A����
		1.0f,			// �[�x�o�b�t�@���N���A����l
		0			// �X�e���V���o�b�t�@���N���A����l(����͊֌W�Ȃ�)
	);
}
// �`��
HRESULT ScreenFlip()
{
	// �o�b�N�o�b�t�@�̕\��
	HRESULT hr = SwapChain->Present(
		0,	// ��ʂ������ɍX�V����
		0	// ��ʂ����ۂɍX�V����
	);

	return hr;
}

//�I�����������
static VOID Cleanup()
{
	SwapChain->Release();
	RenderTargetView->Release();
	DeviceContext->Release();
	Device->Release();
}

//���b�Z�[�W�v���V�[�W��
static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY://�I����
		Cleanup();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
