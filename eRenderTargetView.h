#pragma once


class eRenderTarget {
public:
	bool Create(ID3D11Device* pDevice, IDXGISwapChain* pSwapChain, bool window = false) {
		HRESULT hr;

		if (window == true) {
			pSwapChain->GetBuffer(0, IID_PPV_ARGS(&m_pTexture2D));
			hr = pDevice->CreateRenderTargetView(m_pTexture2D, NULL, &m_pRenderTargetView);
			if (!CheckHresult(hr, "Target View Camera Error")) {
				Release();
				return false;
			}
		}
		else if (window == false) {
			D3D11_TEXTURE2D_DESC textureDesk{}; //Render Target Структура
			textureDesk.Width = 1280;
			textureDesk.Height = 700;
			textureDesk.MipLevels = 1U;
			textureDesk.ArraySize = 1U;
			textureDesk.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			textureDesk.SampleDesc.Count = 1U;
			textureDesk.Usage = D3D11_USAGE_DEFAULT;
			textureDesk.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			textureDesk.CPUAccessFlags = 0U;
			textureDesk.MiscFlags = 0U;


			hr = pDevice->CreateTexture2D(&textureDesk, NULL, &m_pTexture2D);
			if (!CheckHresult(hr, "Create Texture Error")) {
				Release();
				return false;
			}

			D3D11_RENDER_TARGET_VIEW_DESC renderDesk{};
			renderDesk.Format = textureDesk.Format;
			renderDesk.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			renderDesk.Texture2D.MipSlice = 0U;

			hr = pDevice->CreateRenderTargetView(m_pTexture2D, &renderDesk, &m_pRenderTargetView); //Ставим буффер
			if (!CheckHresult(hr, "Target View Error")) {
				Release();
				return false;
			}

			D3D11_SHADER_RESOURCE_VIEW_DESC shaderViewDesk{}; //Описание
			shaderViewDesk.Format = textureDesk.Format;
			shaderViewDesk.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderViewDesk.Texture2D.MostDetailedMip = 0;
			shaderViewDesk.Texture2D.MipLevels = 1;


			hr = pDevice->CreateShaderResourceView(m_pTexture2D, &shaderViewDesk, &m_pResourceView); //Ставим буффер
			if (!CheckHresult(hr, "Target View Error")) {
				Release();
				return false;
			}
		}
		SafeRelease(&m_pTexture2D);
		return true;
	}

	void Set(ID3D11DeviceContext* pDeviceContext) {
		pDeviceContext->OMSetRenderTargets(0, &m_pRenderTargetView, nullptr);
	}

	void CreateTexture(ID3D11DeviceContext* pDeviceContext) {
		pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
	}

	ID3D11RenderTargetView* getRenderTarget() {
		return m_pRenderTargetView;
	}

	ID3D11ShaderResourceView* getShaderResource() {
		return m_pResourceView;
	}

	void Release() {
		SafeRelease(&m_pTexture2D);
		SafeRelease(&m_pResourceView);
		SafeRelease(&m_pRenderTargetView);
	}
private:
	ID3D11Texture2D* m_pTexture2D;
	ID3D11ShaderResourceView* m_pResourceView;
	ID3D11RenderTargetView* m_pRenderTargetView;
};