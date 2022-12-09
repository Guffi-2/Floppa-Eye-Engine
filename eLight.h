#pragma once


struct eLight {
	struct sLight {
		D3DXVECTOR3 pos;
		float range;
		D3DXVECTOR3 dir;
		float cone;
		D3DXVECTOR3 att;
		float pad2;
		D3DXVECTOR4 ambient;
		D3DXVECTOR4 diffuse;
	};

	struct PerFrame {
		sLight light;
	} perFrameConst;
public:
	bool Create(ID3D11Device* pDevice, eShader defaultShader) {

		m_light.pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_light.dir = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		m_light.range = 1000.0f;
		m_light.cone = 20.0f;
		m_light.att = D3DXVECTOR3(0.4f, 0.02f, 0.000f);
		m_light.ambient = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f);
		m_light.diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

		m_LightBuffer.CreateBuffer(pDevice, NULL, sizeof(PerFrame), FLOPPA_CONSTANT);

		D3D11_RASTERIZER_DESC cmdres{};
		cmdres.FillMode = D3D11_FILL_SOLID;
		cmdres.CullMode = D3D11_CULL_BACK;
		cmdres.FrontCounterClockwise = true;
		pDevice->CreateRasterizerState(&cmdres, &pRastStateOn);
		cmdres.FrontCounterClockwise = false;
		pDevice->CreateRasterizerState(&cmdres, &pRastStateOff);


		D3D11_SAMPLER_DESC sampleDesk{};
		sampleDesk.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampleDesk.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampleDesk.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampleDesk.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampleDesk.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampleDesk.MinLOD = 0;
		sampleDesk.MaxLOD = D3D11_FLOAT32_MAX;
		pDevice->CreateSamplerState(&sampleDesk, &pSampleState);
		return true;
	}

	void Render(ID3D11DeviceContext* pDeviceContext) {
		const auto LIGHTBUFFER = m_LightBuffer.getBuffer();
		perFrameConst.light = m_light;
		pDeviceContext->UpdateSubresource(m_LightBuffer.getBuffer(), 0, NULL, &perFrameConst, 0, 0);
		pDeviceContext->RSSetState(pRastStateOff);
		pDeviceContext->PSSetConstantBuffers(0, 1, &LIGHTBUFFER);
		pDeviceContext->OMSetBlendState(0, 0, 0xffffffff);
	}

	void Release() {
		SafeRelease(&pSampleState);
		SafeRelease(&pRastStateOn);
		SafeRelease(&pRastStateOff);
		m_LightBuffer.Release();
	}
private:
	ID3D11RasterizerState* pRastStateOn;
	ID3D11RasterizerState* pRastStateOff;
	ID3D11SamplerState* pSampleState;
	eBuffer m_LightBuffer;
	sLight m_light;
};