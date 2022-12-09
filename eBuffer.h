#pragma once


struct eBuffer {
public:
	~eBuffer() {
		Release();
	}
	/// <summary>
	/// Creating buffer
	/// </summary>
	/// <param name="pDevice">Direct Device</param>
	/// <param name="pBufferData">Buffer Data</param>
	/// <param name="Type">Type</param>
	bool CreateBuffer(ID3D11Device* pDevice, void* pBufferData, UINT sizeOfBuffer, UINT Type) {
		HRESULT hr = S_OK;
		UINT fBind = D3D11_BIND_VERTEX_BUFFER;
		if (Type == FLOPPA_VERTEX) { fBind = D3D11_BIND_VERTEX_BUFFER; }
		if (Type == FLOPPA_INDEX) { fBind = D3D11_BIND_INDEX_BUFFER; }
		if (Type == FLOPPA_CONSTANT) { fBind = D3D11_BIND_CONSTANT_BUFFER; }
		D3D11_BUFFER_DESC bufferDesk{}; //Заполняем параметры буффера
		bufferDesk.ByteWidth = sizeOfBuffer;
		bufferDesk.Usage = D3D11_USAGE_DEFAULT;
		bufferDesk.BindFlags = fBind;
		bufferDesk.MiscFlags = 0U;
		bufferDesk.StructureByteStride = 0U;
		bufferDesk.CPUAccessFlags = 0U;

		D3D11_SUBRESOURCE_DATA subresourceData{}; //Ресурс буффера
		subresourceData.pSysMem = pBufferData;
		subresourceData.SysMemPitch = 0U;
		subresourceData.SysMemSlicePitch = 0U;
		if (Type == FLOPPA_CONSTANT) {
			hr = pDevice->CreateBuffer(&bufferDesk, nullptr, &pBuffer); //Конст создание буффера
		}
		else {
			hr = pDevice->CreateBuffer(&bufferDesk, &subresourceData, &pBuffer); //Создание буффера
		}
		if (hr != S_OK) {
			return false;
		}
		return true;
	}

	ID3D11Buffer* getBuffer() {
		return pBuffer;
	}

	void Release() {
		SafeRelease(&pBuffer);
	}
private:
	ID3D11Buffer* pBuffer;
};