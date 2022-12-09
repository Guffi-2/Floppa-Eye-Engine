#include <iostream>
#include "FloppaEyeEngine.h"

eFloppaEyeEngine engine;
eHelpedObject object;
eHelpedObject objectTest;
eMatrix matrix;
eLight light;
eShader defShader;
eCameraObject cam;

//eRenderTarget tar;

void CameraMove()
{
	static const float speed = 0.002f;
	static const float speed_m = 0.0001f;

	// Повороты камеры
	if (GetAsyncKeyState(VK_LEFT) & 0x8000f) { matrix.setRotation(0.0f, -speed_m, 0); }
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000f) { matrix.setRotation(0.0f, speed_m, 0); }
	if (GetAsyncKeyState(VK_UP) & 0x8000f) { matrix.setRotation(-speed_m, 0.0f, 0); }
	if (GetAsyncKeyState(VK_DOWN) & 0x8000f) { matrix.setRotation(speed_m, 0.0f, 0); }

	// Вперед, назад, лево, право
	if (GetAsyncKeyState('W') & 0x8000f) { matrix.setPosition(matrix.getForward() * speed); }
	if (GetAsyncKeyState('S') & 0x8000f) { matrix.setPosition(matrix.getBackward() * speed); }
	if (GetAsyncKeyState('A') & 0x8000f) { matrix.setPosition(matrix.getLeft() * speed); }
	if (GetAsyncKeyState('D') & 0x8000f) { matrix.setPosition(matrix.getRight() * speed); }

	// Вверх, в низ
	if (GetAsyncKeyState(VK_SPACE)) { matrix.setPosition(0.0f, speed, 0.0f); }
	if (GetAsyncKeyState('Z')) { matrix.setPosition(0.0f, -speed, 0.0f); }
}

void Init() {
	defShader.AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	defShader.AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	defShader.AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);

	defShader.CreateShader(engine.getDevice(), "Effects.fx", "VS", "vs_4_0", FLOPPA_VERTEX);
	defShader.CreateShader(engine.getDevice(), "Effects.fx", "PS", "ps_4_0", FLOPPA_PIXEL); //Создание шейдера
	matrix.CreateMatrix(engine.getDevice());
	object.CreateObject(engine.getDevice());
	object.setMatrix(&matrix);	
	objectTest.CreateObject(engine.getDevice());
	objectTest.setMatrix(&matrix);
	light.Create(engine.getDevice(), defShader);
	cam.CreateObject(engine.getDevice(), engine.getSwapChain(), eWINDOW | eTEXTURE);
}

void Render() { 

	light.Render(engine.getDeviceContext());
	cam.DrawTexture(engine.getDeviceContext());

	object.RenderObject(engine.getDevice(), engine.getDeviceContext(), defShader, { 0.0f, 0.0f, 0.0f });
	cam.DrawWindow(engine.getDeviceContext());
	objectTest.setCamera(cam);
	objectTest.RenderObject(engine.getDevice(), engine.getDeviceContext(), defShader, { 0.0f, 0.0f, 0.0f });
}

int main() {
	if (!engine.Init()) {
		Release();
		return -1;
	}
	Init(); //Custom
	bool WhileWindowIsOpen = true;

	while (WhileWindowIsOpen) {
		MSG msg;
		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {

			TranslateMessage(&msg);
			DispatchMessageA(&msg);

			if (msg.message == WM_QUIT) { WhileWindowIsOpen = false; break; }
		}
		CameraMove();
		engine.StartRender();
		Render(); //Custom
		engine.EndRender();

	}
	object.Release();
	light.Release();
	engine.Release();


	return 0;
}

//stepPhysics();
//PxScene* scene;
//PxGetPhysics().getScenes(&scene, 1);
//PxU32 nbActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
//if (nbActors)
//{
//	std::vector<PxRigidActor*> actors(nbActors);
//	scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
//	renderActors(&actors[0], static_cast<PxU32>(actors.size()));
//}