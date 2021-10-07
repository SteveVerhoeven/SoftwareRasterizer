#include "Scene_Week7_TukTuk.h"

// Elite Includes
#include "ERenderer.h"
#include "ETimer.h"

// Features includes
#include "Scenegraph.h"

void Scene_Week7_TukTuk::Initialize(const uint32_t screenWidth, const uint32_t screenHeight, SDL_Window* pWindow, const Elite::FPoint3& camPos)
{
	Scene::Initialize(screenWidth, screenHeight, pWindow, camPos);

	CreateScene();

	// To prevent code from trying to access textures when a scene does not uses them
	m_UseTextures = true;
	m_UseDiffuseTextureOnly = true;
	m_UseAllTextures = false;

	m_UpdateMesh = true;
}
void Scene_Week7_TukTuk::Update(const float elapsedSec)
{
	Scene::Update(elapsedSec);
}
void Scene_Week7_TukTuk::Render() const
{
	Scene::Render();
}

void Scene_Week7_TukTuk::CreateScene()
{
	// Add mesh & texture
	TriangleMesh* pMesh{ new TriangleMesh() };
	pMesh->InitializeMesh("tuktuk", "./Resources/tuktuk.obj", PrimitiveTopology::TRIANGLE_LIST);
	pMesh->InitializeTexture("./Resources/tuktuk.png", Texture::TextureType::DIFFUSE);
	pMesh->InitializeTexture("./Resources/vehicle_normal.png", Texture::TextureType::NORMAL);
	pMesh->InitializeTexture("./Resources/vehicle_gloss.png", Texture::TextureType::GLOSS);
	pMesh->InitializeTexture("./Resources/vehicle_specular.png", Texture::TextureType::SPECULAR);
	m_pSceneGraph->AddMeshToGraph(pMesh);
}