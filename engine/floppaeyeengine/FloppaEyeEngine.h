#pragma once

#include "eEngine.h"
#include "eFloppaPhys.h"

class eHelpedEngine {
private:
	using m_renderMethod = floppaEye;
public:
	void StartEngine() {
		if (!m_Engine.Init()) {
			Release();
			return;
		}
		
		for (int i = 0; i < m_methodVectorInit.size(); i++) {
			m_methodVectorInit[i]();
		}
		
		bool WhileWindowIsOpen = true;

		while (WhileWindowIsOpen) {
			MSG msg;
			while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {

				TranslateMessage(&msg);
				DispatchMessageA(&msg);

				if (msg.message == WM_QUIT) { WhileWindowIsOpen = false; break; }
			}



			m_Engine.Render(m_methodVectorTicket, m_methodVector);
		}
	}

	void AddMethodRender(m_renderMethod getMethod, bool ticket = false) {
		m_methodVector.push_back(getMethod);
	}

	void AddMethodInit(m_renderMethod getMethod) {
		m_methodVectorInit.push_back(getMethod);
	}

	void AddTicketMethodRender(m_renderMethod getMethod, bool ticket = false) {
		m_methodVectorTicket.push_back(getMethod);
	}

	eFloppaEyeEngine GetEngine() {
		return m_Engine;
	}

	void Release() {
		m_Engine.Release();
		m_methodVector.clear();
	}
private:
	eFloppaEyeEngine m_Engine;
	std::vector<m_renderMethod> m_methodVectorInit;
	std::vector<m_renderMethod> m_methodVector;
	std::vector<m_renderMethod> m_methodVectorTicket;
};



class eHelpedObject {
public:
	void CreateObject(ID3D11Device* pDevice) {
		BuffVer vertices[] =
		{
			// координаты X, Y, Z      текстура U, V  нормаль X, Y, Z
			{{0.0f, 1.0f, 0.0f}, { 1.0f, 0.0f} , {1.0f, 0.0f, 0.0f}},
			{ { 1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f} , { 0.0f, 1.0f, 0.0f } },
			{ { -1.0f, -1.0f, 0.0f },{ 1.0f, 0.0f} ,{ 0.0f, 0.0f, 1.0f } }
		};

		m_Buffer.CreateBuffer(pDevice, vertices, sizeof(vertices), CB_VERTEX); //Создание буффера
		m_Texture.CreateTexture(pDevice, "C:\\Users\\marko\\Downloads\\asd.png");
	}

	void Render(ID3D11DeviceContext* pDeviceContext) {
		const auto setTexture = m_Texture.getTexture(); //CONST !!! НУЖНО ПОМЕНЯТЬ
		const auto buffer = m_Buffer.getBuffer(); //CONST !!! НУЖНО ПОМЕНЯТЬ
		UINT stride = sizeof(BuffVer); //Размер буффера
		UINT offsets = 0; //Смещение(не используется)
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pDeviceContext->PSSetShaderResources(0, 1, &setTexture); //Ставим текстуру
		pDeviceContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offsets); //Ставим Вертекс
		pDeviceContext->IASetInputLayout(shaderVertex.GetInputLayout()); //Ставим шейдеры
		pDeviceContext->VSSetShader(shaderVertex.GetVertexShader(), nullptr, 0);
		pDeviceContext->PSSetShader(shaderVertex.GetPixelShader(), nullptr, 0);
		pDeviceContext->Draw(8, 0); //Рисуем
	}

	void Release() {
		m_Buffer.Release();
		m_Texture.Release();
	}
private:
	eBuffer m_Buffer;
	eTexture m_Texture;
};