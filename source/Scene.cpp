#include "Scene.h"

// Features includes
#include "Scenegraph.h"

// Elite includes
#include "ERenderer.h"

Scene::Scene()
	: m_pERenderer{ nullptr }
	, m_pSceneGraph{ nullptr }
	, m_UseTextures{ false }
	, m_UseDiffuseTextureOnly{ false }
	, m_UseAllTextures{ false }
{}

Scene::~Scene()
{
	DELETEPOINTER(m_pERenderer);
	DELETEPOINTER(m_pSceneGraph);
}

void Scene::Initialize(const uint32_t screenWidth, const uint32_t screenHeight, SDL_Window* pWindow, const Elite::FPoint3& camPos)
{
	m_pERenderer = new Elite::Renderer(pWindow);
	m_pSceneGraph = new Scenegraph();
	m_pSceneGraph->Initialize(screenWidth, screenHeight, camPos);
}

void Scene::Update(const float elapsedSec)
{
	m_pSceneGraph->Update(elapsedSec, m_UpdateMesh);
}

void Scene::Render() const
{
	std::vector<TriangleMesh*> meshes{ m_pSceneGraph->GetTriangleMeshVector() };

	size_t meshesSize{ meshes.size() };
	for (size_t i{}; i < meshesSize; ++i)
	{
		m_pERenderer->Render(m_pSceneGraph, *meshes[i], m_UseTextures, m_UseDiffuseTextureOnly, m_UseAllTextures);
	}
}