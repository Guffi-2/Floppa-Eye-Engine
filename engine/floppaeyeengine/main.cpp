#include <iostream>
#include "FloppaEyeEngine.h"

eHelpedEngine game;
eHelpedObject object;

void Init() {
	object.CreateObject(game.GetEngine().getDevice());
}
void Render() {
	object.Render(game.GetEngine().getDeviceContext());
}

int main() {
	game.AddMethodInit(Init);
	game.AddMethodRender(Render);
	game.StartEngine();
	game.Release();
	object.Release();



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