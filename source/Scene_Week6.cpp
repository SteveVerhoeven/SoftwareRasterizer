#include "Scene_Week6.h"

// Elite Includes
#include "ERenderer.h"
#include "ETimer.h"

// Features includes
#include "Scenegraph.h"

void Scene_Week6::Initialize(const uint32_t screenWidth, const uint32_t screenHeight, SDL_Window* pWindow, const Elite::FPoint3& camPos)
{
	Scene::Initialize(screenWidth, screenHeight, pWindow, camPos);

	CreateScene();

	// To prevent code from trying to access textures when a scene does not uses them
	m_UseTextures = false;
	m_UseDiffuseTextureOnly = false;
	m_UseAllTextures = false;

	m_UpdateMesh = false;
}
void Scene_Week6::Update(const float elapsedSec)
{
	Scene::Update(elapsedSec);
}
void Scene_Week6::Render() const
{
	Scene::Render();
}

void Scene_Week6::CreateScene()
{
	Elite::RGBColor white{ Elite::RGBColor(1, 1, 1) };
	Elite::RGBColor red{   Elite::RGBColor(1, 0, 0) };
	Elite::RGBColor green{ Elite::RGBColor(0, 1, 0) };
	Elite::RGBColor blue{  Elite::RGBColor(0, 0, 1) };
	const float depthClose{ -1.9f };
	const float depthFar{ -2.0 };
	
	// Add mesh & texture
	TriangleMesh* pMesh{ new TriangleMesh() };
	m_pSceneGraph->AddMeshToGraph(pMesh);

	// Create Index buffer
	std::vector<size_t> iBuffer{ 0, 1, 2,
								 3, 4, 5 };

	// Create Vertex buffer
	std::vector<Vertex> vBuffer{};
	vBuffer.push_back(Vertex{ Elite::FPoint3(0.0f,  2.0f, depthClose), white, Elite::FVector2(1, 1), Elite::FVector3(0, 0, 0) });
	vBuffer.push_back(Vertex{ Elite::FPoint3(-1.0f,  0.0f, depthClose), white, Elite::FVector2(1, 1), Elite::FVector3(0, 0, 0) });
	vBuffer.push_back(Vertex{ Elite::FPoint3(1.0f,  0.0f, depthClose), white, Elite::FVector2(1, 1), Elite::FVector3(0, 0, 0) });
	vBuffer.push_back(Vertex{ Elite::FPoint3(0.0f,  4.0f, depthFar), red  , Elite::FVector2(0.5f, 0), Elite::FVector3(0, 0, 0) });
	vBuffer.push_back(Vertex{ Elite::FPoint3(-3.0f, -2.0f, depthFar), green, Elite::FVector2(0, 1), Elite::FVector3(0, 0, 0) });
	vBuffer.push_back(Vertex{ Elite::FPoint3(3.0f, -2.0f, depthFar), blue , Elite::FVector2(1, 1), Elite::FVector3(0, 0, 0) });

	pMesh->InitializeBuffers(iBuffer, vBuffer, PrimitiveTopology::TRIANGLE_LIST);
}
