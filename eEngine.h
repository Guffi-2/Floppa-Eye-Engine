#pragma once

#include <Windows.h>
#include <vector>
#include <map>
#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10math.h>
#include <D3Dcompiler.h>
#include <D3D11Shader.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3DCompiler.lib")

template <class T>
void SafeRelease(T** prt) { //Безопасное удаление типа
	if (*prt) {
		(*prt)->Release();
		*prt = nullptr;
	}
}

#include "htTicks.h"

#include "eDefines.h"

#include "eMessage.h"
#include "eBuffer.h"
#include "eWindow.h"
#include "eShader.h"
#include "eTexture.h"
#include "eMatrix.h"
#include "eLight.h"
#include "eRenderTargetView.h"

struct BuffVer {
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 textrue;
};


using floppaEye = void(*)();

/// <summary>
/// ENGINE
/// </summary>
class eFloppaEyeEngine { //Движоккк
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	eRenderTarget m_renderTarget;
	eWindow m_eWindowEngine{}; //Окно
	

	using m_renderMethod = floppaEye; //метод

public:
	eFloppaEyeEngine() {

	}

	~eFloppaEyeEngine() {
	}

	ID3D11Device* getDevice() {
		return m_pDevice;
	}
	ID3D11DeviceContext* getDeviceContext(){
		return m_pDeviceContext;
	}
	IDXGISwapChain* getSwapChain() {
		return m_pSwapChain;
	}
	eRenderTarget getRenderTargetView() {
		return m_renderTarget;
	}

	bool Init() {
		HRESULT hr = S_OK;
		m_eWindowEngine.eCreateWindow();
		m_eWindowEngine.eOpenWindow();

		D3D_FEATURE_LEVEL featureLevel[] = { //Какие DirectX я использую (10.0, 10.1, 11)
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_11_0
		};
		DXGI_SWAP_CHAIN_DESC swapDesk{}; //SwapChain параметры
		swapDesk.BufferCount = 1;
		swapDesk.BufferDesc.Width = 1280;
		swapDesk.BufferDesc.Height = 700;
		swapDesk.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesk.BufferDesc.RefreshRate.Numerator = 60;
		swapDesk.BufferDesc.RefreshRate.Denominator = 0;
		swapDesk.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesk.Flags = 0;
		swapDesk.Windowed = TRUE;
		swapDesk.OutputWindow = m_eWindowEngine.hWnd;
		swapDesk.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapDesk.SampleDesc.Count = 1;
		swapDesk.SampleDesc.Quality = 0;

		hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevel, ARRAYSIZE(featureLevel), D3D11_SDK_VERSION, &swapDesk, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext); //Создание девайса и swap chain
		if (!CheckHresult(hr, "Device Error")) {
			Release();
			return false;
		}


		D3D11_VIEWPORT viewPort{}; //Область, где рисует
		viewPort.MaxDepth = 1.0f;
		viewPort.Width = (float)1280;
		viewPort.Height = (float)700;
		m_pDeviceContext->RSSetViewports(1, &viewPort); //Ставим ViewPort

		return true;
	}

	void StartRender() {

	}

	void EndRender() {
		m_pSwapChain->Present(0, 0);
	}
	void Release() {
		SafeRelease(&m_pDevice);
		SafeRelease(&m_pDeviceContext);
		SafeRelease(&m_pSwapChain);
		m_renderTarget.Release();
		m_eWindowEngine.eRelease();
	}
private:
};