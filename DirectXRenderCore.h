#pragma once


#include <d3d11.h>
#include "WindowsApplication.h"
#include <wrl\wrappers\corewrappers.h>
#include <wrl\client.h>
#include "Utility.h"
#include "IRenderEngine.h"
#include "ErrorCodes.h"
#include <vector>
#include <map>
#include "RenderObject.h"
#include "KongEffect.h"
#include "Camera.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;


class DirectXRenderCore : public IRenderEngine
{
public:

	DirectXRenderCore();

	virtual UINT Initialize(HWND, UINT, UINT) override; 
	virtual void SubmitObject(const EngineObject&) override;
	virtual UINT Render() override;
	Kong::Camera& GetCamera() override;


private:

	void mCreateSwapChainAndDevice();
	void mCreateRenderTargetsAndDepthBuffers();
	void mCreateViewport();
	void mCreateEffects();


private:
	std::vector<RenderObject*> mPipelineObjectList;
	std::map<unsigned int, KongEffect*> mEffects;

//framework base
private:

	//OS Window
	HWND mWindowHandle; 
	UINT mWindowClientWidth;
	UINT mWindowClientHeight; 

	
	ComPtr<ID3D11Device> mDevice;
	ComPtr<ID3D11DeviceContext> mContext;
	ComPtr<IDXGISwapChain> mSwapChain;

	//Draw Surfaces
	ComPtr<ID3D11RenderTargetView> mBackBufferView;
	ComPtr<ID3D11Texture2D> mDepthStencilTexture;
	ComPtr<ID3D11DepthStencilView> mDepthStencilView;
	ComPtr<ID3D11DepthStencilState> mDepthStencilState;

	D3D11_VIEWPORT mViewport;
	Kong::Camera mCamera;
};




