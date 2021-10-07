#include "Scene_Week8_Vehicle.h"

// Elite Includes
#include "ERenderer.h"
#include "ETimer.h"

// Features includes
#include "Scenegraph.h"

void Scene_Week8_Vehicle::Initialize(const uint32_t screenWidth, const uint32_t screenHeight, SDL_Window* pWindow, const Elite::FPoint3& camPos)
{
	Scene::Initialize(screenWidth, screenHeight, pWindow, camPos);

	CreateScene();

	// To prevent code from trying to access textures when a scene does not uses them
	m_UseTextures = true;
	m_UseDiffuseTextureOnly = false;
	m_UseAllTextures = true;

	m_UpdateMesh = true;
}
void Scene_Week8_Vehicle::Update(const float elapsedSec)
{
	Scene::Update(elapsedSec);
}
void Scene_Week8_Vehicle::Render() const
{
	Scene::Render();
}

void Scene_Week8_Vehicle::CreateScene()
{
	// Add mesh & texture
	TriangleMesh* pMesh{ new TriangleMesh() };
	pMesh->InitializeMesh("vehicle", "./Resources/vehicle.obj", PrimitiveTopology::TRIANGLE_LIST);
	pMesh->InitializeTexture("./Resources/vehicle_diffuse.png", Texture::TextureType::DIFFUSE);
	pMesh->InitializeTexture("./Resources/vehicle_normal.png", Texture::TextureType::NORMAL);
	pMesh->InitializeTexture("./Resources/vehicle_gloss.png", Texture::TextureType::GLOSS);
	pMesh->InitializeTexture("./Resources/vehicle_specular.png", Texture::TextureType::SPECULAR);
	m_pSceneGraph->AddMeshToGraph(pMesh);
}