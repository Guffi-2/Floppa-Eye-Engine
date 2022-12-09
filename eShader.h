#pragma once


/// <summary>
/// Engine Shader
/// </summary>
struct eShader {
public:
	void AddInputLayout(const char* semanticName, DXGI_FORMAT elementFormat) {
		D3D11_INPUT_ELEMENT_DESC elementDesk{}; //Структура
		elementDesk.AlignedByteOffset = elementDeskVector.size() ? D3D11_APPEND_ALIGNED_ELEMENT : 0;
		elementDesk.Format = elementFormat;
		elementDesk.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesk.SemanticName = semanticName;
		elementDeskVector.push_back(elementDesk);
	}
	bool CreateShader(ID3D11Device* pDevice, const char* path, const char* name, const char* profile, short type) {
		static int f;
		f++;
		HRESULT hr = S_OK;
		ID3DBlob* pBlobShader = nullptr;
		D3DX11CompileFromFileA(path, NULL, NULL, name, profile, D3DCOMPILE_ENABLE_STRICTNESS, NULL, NULL, &pBlobShader, NULL, NULL);
		if (type == FLOPPA_VERTEX) { //Проверка на тип и загрузка
			hr = pDevice->CreateVertexShader(pBlobShader->GetBufferPointer(), pBlobShader->GetBufferSize(), NULL, &pVertexShader);
			if (hr != S_OK) {
				return false;
			}
			hr = pDevice->CreateInputLayout(elementDeskVector.data(), elementDeskVector.size(), pBlobShader->GetBufferPointer(), pBlobShader->GetBufferSize(), &pInputLayout);
			if (hr != S_OK) {
				return false;
			}
		}
		if (type == FLOPPA_PIXEL) {
			hr = pDevice->CreatePixelShader(pBlobShader->GetBufferPointer(), pBlobShader->GetBufferSize(), NULL, &pPixelShader);
			if (hr != S_OK) {
				return false;
			}
		}
		SafeRelease(&pBlobShader);
		return true;
	}

	ID3D11VertexShader* GetVertexShader() { return pVertexShader; }
	ID3D11PixelShader* GetPixelShader() { return pPixelShader; }
	ID3D11InputLayout* GetInputLayout() { return pInputLayout; }

	void Release() {
		SafeRelease(&pVertexShader);
		SafeRelease(&pPixelShader);
		SafeRelease(&pInputLayout);
		elementDeskVector.clear();
	}
private:
	ID3D11VertexShader* pVertexShader;
	ID3D11PixelShader* pPixelShader;
	ID3D11InputLayout* pInputLayout;
	std::vector<D3D11_INPUT_ELEMENT_DESC>elementDeskVector;
};
