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
void SafeRelease(T** prt) { //Безопасное удаление типа (просто я хз как сказать Release по русски)
	if (*prt) {
		(*prt)->Release();
		*prt = nullptr;
	}
}

#include "htTicks.h"

#include "eMessage.h"
#include "eBuffer.h"
#include "eWindow.h"
#include "eShader.h"
#include "eTexture.h"
#include "eMatrix.h"

struct BuffVer {
	D3DXVECTOR3 position;
	D3DXVECTOR2 textrue;
	D3DXVECTOR3 normal;
};


//Временные
eShader shaderVertex;
eBuffer heheebufVer;
eTexture heheTexture;
eMatrix heheMatrix;

using floppaEye = void(*)();

/// <summary>
/// ENGINE
/// </summary>
class eFloppaEyeEngine { //Движоккк
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	eWindow m_eWindowEngine{}; //Окно
	using m_renderMethod = floppaEye; //метод

public:

	~eFloppaEyeEngine() {
		//Release();
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
	ID3D11RenderTargetView* getRenderTargetView() {
		return m_pRenderTargetView;
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
		swapDesk.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
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

		ID3D11Texture2D* piTexture = NULL; //Render Target

		hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&piTexture); //Получение SwapChain буффер
		if (!CheckHresult(hr, "Get Buffer Error")) {
			Release();
			return false;
		}
		hr = m_pDevice->CreateRenderTargetView(piTexture, NULL, &m_pRenderTargetView); //Ставим буффер
		if (!CheckHresult(hr, "Target View Error")) {
			Release();
			return false;
		}

		SafeRelease(&piTexture);

		D3D11_VIEWPORT viewPort{}; //Область, где рисует
		viewPort.MaxDepth = 1.0f;
		viewPort.Width = (float)1280;
		viewPort.Height = (float)700;
		m_pDeviceContext->RSSetViewports(1, &viewPort); //Ставим ViewPort

		shaderVertex.AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
		shaderVertex.AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
		shaderVertex.AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);

		shaderVertex.CreateShader(m_pDevice, "f.shader", "VShader", "vs_4_0", eVertexShader);
		shaderVertex.CreateShader(m_pDevice, "f.shader", "PShader", "ps_4_0", ePixelShader); //Создание шейдера

		auto t1 = shaderVertex.GetInputLayout();
		auto t2 = shaderVertex.GetPixelShader();
		auto t3 = shaderVertex.GetVertexShader();

		//BuffVer vertices[] =
		//{
		//	// координаты X, Y, Z      текстура U, V  нормаль X, Y, Z
		//	{{0.0f, 1.0f, 0.0f}, { 1.0f, 0.0f} , {1.0f, 0.0f, 0.0f}},
		//	{ { 1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f} , { 0.0f, 1.0f, 0.0f } },
		//	{ { -1.0f, -1.0f, 0.0f },{ 1.0f, 0.0f} ,{ 0.0f, 0.0f, 1.0f } }
		//};
		//heheebufVer.CreateBuffer(m_pDevice, vertices, sizeof(vertices), CB_VERTEX); //Создание буффера
		//heheTexture.CreateTexture(m_pDevice, "C:\\Users\\marko\\Downloads\\textures\\Barrel_02_diff_4k.jpg"); //Создание текстуры
		heheMatrix.CreateMatrix(m_pDevice);
	}

	void Render(std::vector<m_renderMethod> TicketsMethods, std::vector<m_renderMethod> methods) {
		float color[4] = { 1.0f, 0.2f, 1.5f, 1.0f };
		m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr); //Ставим Render Target
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color); //Заливаем область цветом

		heheMatrix.ChangePosition(m_pDeviceContext, { 1.0f,1.0f,1.0f }, { 1.0f,1.0f,1.0f }, { 1.0f,1.0f,1.0f });
		heheMatrix.UseWorld(m_pDeviceContext);
	/*	short repeatTicks = 0;
		if (helpTools::EveryTicks(0, "render", &repeatTicks), true) {
			for (int i = 0; i < TicketsMethods.size(); i++) {
				TicketsMethods[i]();
			}
		}*/

		for (int i = 0; i < methods.size(); i++) {
			methods[i]();
		}
		m_pSwapChain->Present(0, 0);
	}
	void Release() {
		SafeRelease(&m_pDevice);
		SafeRelease(&m_pDeviceContext);
		SafeRelease(&m_pSwapChain);
		SafeRelease(&m_pRenderTargetView);
		m_eWindowEngine.eRelease();
	}
private:
};