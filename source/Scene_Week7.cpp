#include "Scene_Week7.h"

// Elite Includes
#include "ERenderer.h"
#include "ETimer.h"

// Features includes
#include "Scenegraph.h"

void Scene_Week7::Initialize(const uint32_t screenWidth, const uint32_t screenHeight, SDL_Window* pWindow, const Elite::FPoint3& camPos)
{
	Scene::Initialize(screenWidth, screenHeight, pWindow, camPos);

	CreateScene();

	// To prevent code from trying to access textures when a scene does not uses them
	m_UseTextures = true;
	m_UseDiffuseTextureOnly = true;
	m_UseAllTextures = false;

	m_UpdateMesh = false;
}
void Scene_Week7::Update(const float elapsedSec)
{
	Scene::Update(elapsedSec);
}
void Scene_Week7::Render() const
{
	Scene::Render();
}

void Scene_Week7::CreateScene()
{
	Elite::RGBColor white{ Elite::RGBColor(1, 1, 1) };
	const float depth{ -2.0 };

	// Add mesh & texture
	TriangleMesh* pMesh{ new TriangleMesh() };
	m_pSceneGraph->AddMeshToGraph(pMesh);

	// Create Index buffer
	std::vector<size_t> iBufferList{ 0, 3, 1, 
								     3, 4, 1, 
								     1, 4, 2, 
								     4, 5, 2, 
								     3, 6, 4, 
								     6, 7, 4, 
								     4, 7, 5, 
								     7, 8, 5 };
	std::vector<size_t> iBufferStrip{ 0, 3, 1, 4, 2, 5, 5, 3, 3, 6, 4, 7, 5, 8 };

	// Create Vertex buffer
	std::vector<Vertex> vBuffer{};
	vBuffer.push_back( Vertex{ Elite::FPoint3( -3.f,  3.f, depth), white, Elite::FVector2(0.0f, 0.0f), Elite::FVector3(0, 0, 0) } );
	vBuffer.push_back( Vertex{ Elite::FPoint3(  0.f,  3.f, depth), white, Elite::FVector2(0.5f, 0.0f), Elite::FVector3(0, 0, 0) } );
	vBuffer.push_back( Vertex{ Elite::FPoint3(  3.f,  3.f, depth), white, Elite::FVector2(1.0f, 0.0f), Elite::FVector3(0, 0, 0) } );
	vBuffer.push_back( Vertex{ Elite::FPoint3( -3.f,  0.f, depth), white, Elite::FVector2(0.0f, 0.5f), Elite::FVector3(0, 0, 0) } );
	vBuffer.push_back( Vertex{ Elite::FPoint3(  0.f,  0.f, depth), white, Elite::FVector2(0.5f, 0.5f), Elite::FVector3(0, 0, 0) } );
	vBuffer.push_back( Vertex{ Elite::FPoint3(  3.f,  0.f, depth), white, Elite::FVector2(1.0f, 0.5f), Elite::FVector3(0, 0, 0) } );
	vBuffer.push_back( Vertex{ Elite::FPoint3( -3.f, -3.f, depth), white, Elite::FVector2(0.0f, 1.0f), Elite::FVector3(0, 0, 0) } );
	vBuffer.push_back( Vertex{ Elite::FPoint3(  0.f, -3.f, depth), white, Elite::FVector2(0.5f, 1.0f), Elite::FVector3(0, 0, 0) } );
	vBuffer.push_back( Vertex{ Elite::FPoint3(  3.f, -3.f, depth), white, Elite::FVector2(1.0f, 1.0f), Elite::FVector3(0, 0, 0) } );
	
	pMesh->InitializeBuffers(iBufferList, iBufferStrip, vBuffer, PrimitiveTopology::TRIANGLE_LIST);
	pMesh->InitializeTexture("./Resources/uv_grid_2.png", Texture::TextureType::DIFFUSE);
	pMesh->InitializeTexture("./Resources/vehicle_normal.png", Texture::TextureType::NORMAL);
	pMesh->InitializeTexture("./Resources/vehicle_gloss.png", Texture::TextureType::GLOSS);
	pMesh->InitializeTexture("./Resources/vehicle_specular.png", Texture::TextureType::SPECULAR);
}