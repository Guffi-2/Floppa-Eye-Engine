#pragma once

struct eMatrix {
public:
	void CreateMatrix(ID3D11Device* pDevice) {
		m_matrixBuffer.CreateBuffer(pDevice, nullptr, sizeof(ConstMatrix), FLOPPA_CONSTANT); //�������� ������ �������
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
		D3DXMATRIX mt = (pos);
		D3DXMatrixTranspose(&m1, &mt);
		D3DXMatrixTranspose(&m2, &m_viewMatrix);
		D3DXMatrixTranspose(&m3, &m_projectionMatrix);

		ConstMatrix cmatrix;
		cmatrix.World = m1;
		cmatrix.View = m2;
		cmatrix.Projection = m3;

		pDeviceContext->UpdateSubresource(m_matrixBuffer.getBuffer(), 0, NULL, &cmatrix, 0, 0);
	}

	void Release() {
		m_matrixBuffer.Release();
	}

	D3DXMATRIX getWorld() {
		return m_worldMatrix;
	}

	D3DXMATRIX getView() {
		return m_viewMatrix;
	}

	D3DXMATRIX getProj() {
		return m_projectionMatrix;
	}

	eBuffer getBuffer() {
		return m_matrixBuffer;
	}

	// ��������� ������� ������ �� x, y, z
	void setPosition(float x, float y, float z) {
		m_pos.x = x;
		m_pos.y = y;
		m_pos.z = z;

		m_posVector += m_pos;

		update_view_matrix();
	}

	// ���������� ������ ��������� ������� ������
	void setPosition(const D3DXVECTOR3& pos) {
		m_posVector += pos;

		update_view_matrix();
	}

	// ��������� �������� ������
	void setRotation(float x, float y, float z) {
		m_rot.x += x;
		m_rot.y += y;
		m_rot.z += z;

		update_view_matrix();
	}

	// ������� ��� ���������� ������������ ������ � ������ �������
	const D3DXVECTOR3& getForward() const { return m_forwardVector; }
	const D3DXVECTOR3& getBackward() const { return m_backwardVector; }
	const D3DXVECTOR3& getLeft() const { return m_leftVector; }
	const D3DXVECTOR3& getRight() const { return m_rightVector; }

private:
	D3DXVECTOR3 m_pos, m_rot;
	D3DXVECTOR3 m_posVector;

	D3DXVECTOR3 m_forwardVector;
	D3DXVECTOR3 m_backwardVector;
	D3DXVECTOR3 m_leftVector;
	D3DXVECTOR3 m_rightVector;

	void update_view_matrix() {
		// ��������� �������� ������
		static const D3DXVECTOR3 vAt = { 0.0f, 0.0f, 1.0f };
		static const D3DXVECTOR3 vUp = { 0.0f, 1.0f, 0.0f };

		static const D3DXVECTOR3 forwardVec = { 0.0f, 0.0f, 1.0f };
		static const D3DXVECTOR3 bacwardVec = { 0.0f, 0.0f, -1.0f };
		static const D3DXVECTOR3 leftVec = { -1.0f, 0.0f, 0.0f };
		static const D3DXVECTOR3 rightVec = { 1.0f, 0.0f, 0.0f };

		// ������� ������� �������
		D3DXMATRIX rotationMatrix;
		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, m_rot.y, m_rot.x, m_rot.z); // pitch , yaw , roll

		// ���� �������
		D3DXVECTOR3 at;
		D3DXVec3TransformCoord(&at, &vAt, &rotationMatrix);

		// ����������� �����
		D3DXVECTOR3 up;
		D3DXVec3TransformCoord(&up, &vUp, &rotationMatrix);

		at += m_posVector;

		// �������� ������� �������
		D3DXMatrixLookAtLH(&m_viewMatrix, &m_posVector, &at, &up);

		// ������� �������
		D3DXMATRIX vecRotationMatrix;
		D3DXMatrixRotationYawPitchRoll(&vecRotationMatrix, m_rot.y, 0.0f, 0.0f); // yaw , pitch , roll 

		// �������������� ������� � ������, ���� �������� ������ ���� �� ���������
		D3DXVec3TransformCoord(&m_forwardVector, &forwardVec, &vecRotationMatrix);
		D3DXVec3TransformCoord(&m_backwardVector, &bacwardVec, &vecRotationMatrix);
		D3DXVec3TransformCoord(&m_leftVector, &leftVec, &vecRotationMatrix);
		D3DXVec3TransformCoord(&m_rightVector, &rightVec, &vecRotationMatrix);
	}

	eBuffer m_matrixBuffer;

	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_projectionMatrix;

	struct ConstMatrix {
		D3DMATRIX World;
		D3DMATRIX View;
		D3DMATRIX Projection;
	};
};