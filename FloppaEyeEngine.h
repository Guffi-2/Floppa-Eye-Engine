#pragma once

#include "eEngine.h"
#include "eFloppaPhys.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <unordered_map>

enum HELPED_FLAGS_ : short {
	eWINDOW = 1 << 1,
	eTEXTURE = 1 << 2
};

typedef short HELP_FLAGS;

class eCameraObject {
public:
	void CreateObject(ID3D11Device* pDevice, IDXGISwapChain* pSwapChain, HELP_FLAGS only) {
		if (only & eWINDOW) {
			m_RenderTargetWindow.Create(pDevice, pSwapChain, true); //Рендер для окна
			WindowIsDone = true;
		}
		if (only & eTEXTURE) {
			m_RenderTargetTexture.Create(pDevice, pSwapChain, false); //Рендер для текстуры
			TextureIsDone = true;
		}
	}

	ID3D11ShaderResourceView* GetTexture() {
		return m_RenderTargetTexture.getShaderResource();
	}

	void DrawWindow(ID3D11DeviceContext* pDeviceContext) {
		const float color[4] = { 1.0f, 0.2f, 1.5f, 1.0f };
		const auto RENDER_TARGET = m_RenderTargetWindow.getRenderTarget();
		pDeviceContext->OMSetRenderTargets(1, &RENDER_TARGET, nullptr); //Ставим Render Target
		pDeviceContext->ClearRenderTargetView(RENDER_TARGET, color); //Заливаем область цветом
	}

	void DrawTexture(ID3D11DeviceContext* pDeviceContext) {
		const float color[4] = { 1.0f, 0.2f, 1.2f, 1.0f };
		const auto RENDER_TARGET = m_RenderTargetTexture.getRenderTarget();
		m_RenderTargetTexture.CreateTexture(pDeviceContext);
		pDeviceContext->ClearRenderTargetView(RENDER_TARGET, color);
	}
	
	void Release() {
		m_RenderTargetTexture.Release();
		m_RenderTargetWindow.Release();
		WindowIsDone = false;
		TextureIsDone = false;
	}

private:
	bool WindowIsDone = false;
	bool TextureIsDone = false;
	eRenderTarget m_RenderTargetTexture;
	eRenderTarget m_RenderTargetWindow;
};

class eHelpedMesh {
private:
	static constexpr uint32_t MAX_BONES = 4;
	const aiScene* g_pScene = nullptr;
	Assimp::Importer importer;
	std::vector<ID3D11Buffer>             g_vBufferMgr;
	std::vector<ID3D11Buffer>             g_iBufferMgr;
	class SkinnedMesh
	{
	public:
		SkinnedMesh() :m_numMeshes(0), m_numBones(0) {};
		uint32_t m_numMeshes;
		std::vector<MeshEntry> m_entries;
		std::unordered_map<std::string, uint32_t> m_boneMap;
		uint32_t m_numBones;
		std::vector<BoneInfo> m_boneInfo;
	};
	std::unique_ptr<SkinnedMesh> g_skinnedMesh = nullptr;
	struct BoneData
	{
		uint32_t ids[MAX_BONES];
		float weights[MAX_BONES];
		unsigned char idx;

		BoneData() :idx(0), ids{ 0,0,0,0 }, weights{ 0.0f,0.0f,0.0f,0.0f } {};

		void insertData(uint32_t id, float weight)
		{
			//assert(idx < MAX_BONES);
			if (idx >= MAX_BONES) return;
			ids[idx] = id;
			weights[idx] = weight;
			idx++;
		};
	};

	struct BoneInfo
	{
		DirectX::XMMATRIX boneOffset;
		DirectX::XMMATRIX finalTransformation;
	};

	struct MeshEntry
	{
		std::vector<DirectX::XMFLOAT3> positions;
		std::vector<DirectX::XMFLOAT3> normals;
		std::vector<DirectX::XMFLOAT2> texCoords;
		std::vector<BoneData> boneData;
		std::vector<uint16_t> indices;
		uint32_t materialIdx;
		uint32_t vBufIdx;
		uint32_t iBufIdx;
	};
public:
	void CreateMesh() {
		struct MeshEntry
		{
			std::vector<D3DXVECTOR3> positions;
			std::vector<D3DXVECTOR3> normals;
			std::vector<D3DXVECTOR2> texCoords;
			std::vector<BoneData> boneData;
			std::vector<uint16_t> indices;
			uint32_t materialIdx;
			uint32_t vBufIdx;
			uint32_t iBufIdx;
		};

		g_pScene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded);

		g_skinnedMesh = make_unique<SkinnedMesh>(SkinnedMesh{});
		assert(g_pScene->mNumMeshes > 0);

		g_skinnedMesh->m_numMeshes = g_pScene->mNumMeshes;

		g_skinnedMesh->m_entries.resize(g_pScene->mNumMeshes);
		g_vBufferMgr.resize(g_pScene->mNumMeshes);
		g_iBufferMgr.resize(g_pScene->mNumMeshes);

		for (uint32_t i = 0; i < g_skinnedMesh->m_numMeshes; i++)
		{
			const aiMesh* pAiMesh = g_pScene->mMeshes[i];
			MeshEntry* pEntry = &g_skinnedMesh->m_entries[i];

			for (uint32_t j = 0; j < pAiMesh->mNumVertices; j++)
			{
				const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
				const aiVector3D* vtex = &(pAiMesh->mVertices[j]);
				const aiVector3D* norm = &(pAiMesh->mNormals[j]);
				const aiVector3D* texc = pAiMesh->HasTextureCoords(0) ? &(pAiMesh->mTextureCoords[0][j]) : &Zero3D;

				pEntry->positions.push_back(DirectX::XMFLOAT3(vtex->x, vtex->y, vtex->z));
				pEntry->normals.push_back(DirectX::XMFLOAT3(norm->x, norm->y, norm->z));
				pEntry->texCoords.push_back(DirectX::XMFLOAT2(texc->x, texc->y));
			}

			for (uint32_t k = 0; k < pAiMesh->mNumFaces; k++)
			{
				const aiFace& face = pAiMesh->mFaces[k];
				assert(face.mNumIndices == 3);
				pEntry->indices.push_back(face.mIndices[0]);
				pEntry->indices.push_back(face.mIndices[1]);
				pEntry->indices.push_back(face.mIndices[2]);
			}

			uint32_t numIndices = static_cast<uint32_t>(pEntry->indices.size());
			unique_ptr<uint16_t[]> iBufferMem = make_unique<uint16_t[]>(sizeof(uint16_t) * numIndices); // 3 position and 2 tex
			uint16_t* iDataPtr = iBufferMem.get();
			pEntry->iBufIdx = i;

			for (uint32_t k = 0; k < numIndices; k++)
			{
				assert((pEntry->indices[k] & 0xFFFF0000) == 0);
				iDataPtr[k] = static_cast<uint16_t>(pEntry->indices[k]);
			}

			hr = CreateBuffer(&g_iBufferMgr[i], D3D11_BIND_INDEX_BUFFER, iBufferMem.get(), sizeof(uint16_t), numIndices, D3D11_USAGE_DEFAULT);
			assert(S_OK == hr);

			pEntry->boneData.resize(pAiMesh->mNumVertices);

			for (uint32_t j = 0; j < pAiMesh->mNumBones; j++)
			{
				aiBone* pBone = pAiMesh->mBones[j];
				string boneName(pBone->mName.data);
				uint32_t boneIndex = 0;

				if (g_skinnedMesh->m_boneMap.find(boneName) == g_skinnedMesh->m_boneMap.end())
				{
					boneIndex = g_skinnedMesh->m_numBones;
					g_skinnedMesh->m_boneMap[boneName] = g_skinnedMesh->m_numBones;
					g_skinnedMesh->m_numBones++;

					BoneInfo bi = {};
					bi.boneOffset = aiToXMMATRIX(pBone->mOffsetMatrix);
					g_skinnedMesh->m_boneInfo.push_back(bi);
				}
				else
				{
					boneIndex = g_skinnedMesh->m_boneMap[boneName];
				}

				for (uint32_t k = 0; k < pBone->mNumWeights; k++)
				{
					uint32_t vid = pBone->mWeights[k].mVertexId;
					float weight = pBone->mWeights[k].mWeight;

					if (weight < 0.0001)
					{
						continue;
					}
					pEntry->boneData[vid].insertData(boneIndex, weight);
				}
			}

			unique_ptr<BlendVertex[]> vBufferMem = make_unique<BlendVertex[]>(sizeof(BlendVertex) * pAiMesh->mNumVertices); // 3 position and 2 tex
			BlendVertex* dataPtr = vBufferMem.get();

			for (uint32_t j = 0; j < pAiMesh->mNumVertices; j++)
			{
				dataPtr[j].Pos = pEntry->positions[j];
				dataPtr[j].Normal = pEntry->normals[j];
				dataPtr[j].Tex = pEntry->texCoords[j];
				memcpy_s(&dataPtr[j].BoneIndices, sizeof(uint32_t) * 4, &pEntry->boneData[j].ids, sizeof(uint32_t) * 4);
				memcpy_s(&dataPtr[j].BoneWeights, sizeof(float) * 4, &pEntry->boneData[j].weights, sizeof(float) * 4);
			}

			hr = CreateBuffer(&g_vBufferMgr[i], D3D11_BIND_VERTEX_BUFFER, vBufferMem.get(), sizeof(BlendVertex), pAiMesh->mNumVertices, D3D11_USAGE_DEFAULT);
			assert(S_OK == hr);

			pEntry->vBufIdx = i;

			pEntry->materialIdx = pAiMesh->mMaterialIndex;
		}
	}
private:
};

class eHelpedObject {
public:
	void CreateObject(ID3D11Device* pDevice) {
		//BuffVer vertices[] =
		//{
		//	// координаты X, Y, Z      текстура U, V  нормаль X, Y, Z
		//	{ { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f} ,{ 0.0f, 0.0f, 0.0f }},
		//	{ { 0.0f, 0.5f, 0.0f }, { 1.0f, 1.0f} ,{ 0.0f, 0.5f, 0.0f } },
		//	{ { 0.5f, 0.0f, 0.0f }, { 1.0f, 1.0f} ,{ 0.5f, 0.0f, 0.0f } },
		//	{ { 0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f} ,{ 0.5f, 0.5f, 0.0f } }
		//};
		BuffVer vertices[] =
		{
			// координаты X, Y, Z      текстура U, V  нормаль X, Y, Z
			{{0.0f, 1.0f, 0.0f},      {0.0f, 1.0f, 0.0f}   , { 0.0f, 0.0f}},
			{ { 1.0f, -1.0f, 0.0f },  { 1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f}},
			{ { -1.0f, -1.0f, 0.0f },{ -1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f}}
		};


		m_Buffer.CreateBuffer(pDevice, vertices, sizeof(vertices), FLOPPA_VERTEX); //Создание буффера
		m_Texture.CreateTexture(pDevice, "C:\\Users\\marko\\Downloads\\asd.png");
	}

	void RenderObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, eShader shader, D3DXVECTOR3 position = { 0.0f, 0.0f, 0.0f }, D3DXVECTOR3 rotation = { 0.0f, 0.0f, 0.0f }, D3DXVECTOR3 scale = { 1.0f, 1.0f, 1.0f }) {
		m_Matrix->ChangePosition(pDeviceContext, position, rotation, scale);
		const auto setTextureMatrix = m_RenderTarget.GetTexture();
		const auto setTexture = m_Texture.getTexture();
		//const auto setTexture = m_Texture.getTexture();
		m_Matrix->UseWorld(pDeviceContext);
		const auto buffer = m_Buffer.getBuffer(); //CONST !!!
		UINT stride = sizeof(BuffVer); //Размер буффера
		UINT offsets = 0; //Смещение(не используется)
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		pDeviceContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offsets); //Ставим Вертекс
		pDeviceContext->IASetInputLayout(shader.GetInputLayout()); //Ставим шейдеры
		pDeviceContext->VSSetShader(shader.GetVertexShader(), nullptr, 0);
		pDeviceContext->PSSetShader(shader.GetPixelShader(), nullptr, 0);
		if (!cameraIsDone) {
			pDeviceContext->PSSetShaderResources(0, 1, &setTexture); //Ставим текстуру
		}
		else {
			pDeviceContext->PSSetShaderResources(0, 1, &setTextureMatrix); //Ставим текстуру
		}


		pDeviceContext->Draw(4, 0); //Рисуем
	}

	void setMatrix(eMatrix* pMatrix) {
		m_Matrix = pMatrix;
	}

	void setCamera(eCameraObject pCamera) {
		m_RenderTarget = pCamera;
		cameraIsDone = true;
	}

	void Release() {
		cameraIsDone = false;
		m_Buffer.Release();
		m_Texture.Release();
	}
private:
	bool cameraIsDone = false;
	eCameraObject m_RenderTarget;
	eMatrix* m_Matrix;
	eBuffer m_Buffer;
	eTexture m_Texture;
};