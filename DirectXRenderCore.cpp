#include "DirectXRenderCore.h"
#include "EffectLibrary.h"

void DirectXRenderCore::mCreateSwapChainAndDevice()
{

	//initialize COM interface to use the WIC imagine functions

#if (_WIN32_WINNT >= 0x0A00 /*_WIN32_WINNT_WIN10*/)
	Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
	if (FAILED(initialize))
		return;// error
#else
	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
		// error
#endif
	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

	//setup swap chain ===================================================
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	//set buffer dimensions and format
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = mWindowClientWidth;
	swapChainDesc.BufferDesc.Height = mWindowClientHeight;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;


	//set refresh rate
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	//sampling settings
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SampleDesc.Count = 1;

	//output window handle
	swapChainDesc.OutputWindow = mWindowHandle;
	swapChainDesc.Windowed = true;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;


	//create device ===================================================
	UINT createDeviceFlags = 0;
#ifdef _DEBUG 
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL d3dFL;

	HandleError(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, 0, 0, D3D11_SDK_VERSION, &swapChainDesc, &mSwapChain, &mDevice, &d3dFL, NULL));
	mDevice->GetImmediateContext(&mContext);

}

void DirectXRenderCore::mCreateRenderTargetsAndDepthBuffers()
{


	// create render target for merger state ===================================================
	ID3D11Texture2D* pBackBuffer = nullptr;
	HandleError(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer));

	//try to create a view to the back buffer
	if (pBackBuffer != nullptr)
		HandleError(mDevice->CreateRenderTargetView(pBackBuffer, NULL, &mBackBufferView));

	//release the back buffer
	pBackBuffer->Release();

	//create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth = { 0 };
	descDepth.Width = mWindowClientWidth;
	descDepth.Height = mWindowClientHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HandleError(mDevice->CreateTexture2D(&descDepth, NULL, &mDepthStencilTexture));

	if (mDepthStencilTexture != nullptr)
		HandleError(mDevice->CreateDepthStencilView(mDepthStencilTexture.Get(), 0, &mDepthStencilView));

	//set up depth-stencil buffer
	D3D11_DEPTH_STENCIL_DESC dsDesc = { 0 };

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	mDevice->CreateDepthStencilState(&dsDesc, &mDepthStencilState);
}

void DirectXRenderCore::mCreateViewport()
{
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = (FLOAT)mWindowClientWidth;
	mViewport.Height = (FLOAT)mWindowClientHeight;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1;
}

void DirectXRenderCore::mCreateEffects()
{
	KongEffect* e = new KongEffectLibrary::BasicEffect(mDevice.Get(), &mCamera);
	mEffects.insert(std::make_pair(KONG_EFFECT::BASIC_EFFECT, e));
}

DirectXRenderCore::DirectXRenderCore():mDevice(nullptr), mContext(nullptr), mSwapChain(nullptr), mBackBufferView(nullptr), mDepthStencilTexture(nullptr), mDepthStencilView(nullptr), mDepthStencilState(nullptr), mWindowHandle(nullptr), mWindowClientHeight(0), mWindowClientWidth(0)
{
	mViewport = { 0 };
}

UINT DirectXRenderCore::Initialize(HWND window, UINT clientWidth, UINT clientHeight)
{

	mWindowHandle = window;
	mWindowClientHeight = clientHeight; 
	mWindowClientWidth = clientWidth; 


	mCreateSwapChainAndDevice();
	mCreateRenderTargetsAndDepthBuffers();
	mCreateViewport();
	mCreateEffects();


	return RENDER_CODE::INIT_OK;
}


void DirectXRenderCore::SubmitObject(const EngineObject& obj)
{
	//check if engine object already exists in the render cache
	for (auto ro : mPipelineObjectList)
		if (ro->GetID() == obj.object_id)
		{
			ro->SetRenderable(true);
			return;
		}

	RenderObject* po = new RenderObject(mDevice.Get(), obj);
	po->ChangeEffect(mEffects[obj.effect_id]);
	po->SetID(obj.object_id);
	mPipelineObjectList.push_back(po);
}

UINT DirectXRenderCore::Render()
{
	mContext->RSSetViewports(1, &mViewport);
	mContext->OMSetRenderTargets(1, mBackBufferView.GetAddressOf(), mDepthStencilView.Get());
	mContext->OMSetDepthStencilState(mDepthStencilState.Get(), 0);

	float clearColor[4] = { 0.0f , 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
	mContext->ClearRenderTargetView(mBackBufferView.Get(), clearColor);
	mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH || D3D11_CLEAR_STENCIL, 1, 0);


	for (auto pipe_object : mPipelineObjectList)
	{
		pipe_object->Draw(mContext.Get());
	}


	mSwapChain->Present(0, 0);

	//set all cache objects to non-renderable 
	for (auto ro : mPipelineObjectList)
		ro->SetRenderable(false);

	return RENDER_CODE::DRAW_OK;
}

Kong::Camera& DirectXRenderCore::GetCamera()
{
	return mCamera;
}

