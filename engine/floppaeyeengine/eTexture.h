#pragma once

struct eTexture {
public:
	bool CreateTexture(ID3D11Device* pDevice, const char* path) {
		HRESULT hr = S_OK;
		hr = D3DX11CreateShaderResourceViewFromFile(pDevice, path, NULL, NULL, &pTexture, NULL); //Загрузка текстуры
		if (hr != S_OK) {
			return false;
		}
		return true;
	}

	ID3D11ShaderResourceView* getTexture() {
		return pTexture;
	}

	void Release() {
		SafeRelease(&pTexture);
	}
private:
	ID3D11ShaderResourceView* pTexture;
};