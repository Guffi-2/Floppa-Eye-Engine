#pragma once

struct eMatrix {
public:
	void CreateMatrix(ID3D11Device* pDevice) {
		m_matrixBuffer.CreateBuffer(pDevice, nullptr, sizeof(ConstMatrix), CB_CONSTANT); //�������� ������ �������
		float screenAspect = (float)1280 / 700; //������ ������������
		D3DXMatrixIdentity(&m_worldMatrix); //�������� ������� �������
		D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, (float)D3DX_PI / 4.0f, screenAspect, 0.01f, 1000.0f); //�������� ����������� (D3DX_PI - ����� ��, 0.01f - ��� ���������, 1000.0f - ����)
		D3DXVECTOR3 eye = { 0.0f, 0.0f, -8.0f };
		D3DXVECTOR3 at = { 0.0f, 0.0f, 0.0f };
		D3DXVECTOR3 up = { 0.0f, 1.0f, 0.0f };
		D3DXMatrixLookAtLH(&m_viewMatrix, &eye, &at, &up); //����-������ ������� ����
	}
	
	void UseWorld(ID3D11DeviceContext* pDeviceContext) {
		static auto MATRIX_CONST = m_matrixBuffer.getBuffer();
		pDeviceContext->VSSetConstantBuffers(0, 1, &MATRIX_CONST); //������ �������
	}

	void ChangePosition(ID3D11DeviceContext* pDeviceContext, D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale) {
		D3DXMATRIX world = m_worldMatrix;
		D3DXMATRIX pos;
		D3DXMatrixTranslation(&pos, position.x, position.y, position.z);

		D3DXMATRIX m1, m2, m3;
		D3DXMatrixTranspose(&m1, &pos);
		D3DXMatrixTranspose(&m2, &m_viewMatrix);
		D3DXMatrixTranspose(&m3, &m_projectionMatrix);

		ConstMatrix cmatrix;
		cmatrix.world = m1;
		cmatrix.view = m2;
		cmatrix.projection = m3;

		pDeviceContext->UpdateSubresource(m_matrixBuffer.getBuffer(), 0, NULL, &cmatrix, 0, 0);
	}

	void Release() {
		m_matrixBuffer.Release();
	}
private:
	eBuffer m_matrixBuffer;

	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_projectionMatrix;

	struct ConstMatrix {
		D3DMATRIX world;
		D3DMATRIX view;
		D3DMATRIX projection;
	};
};