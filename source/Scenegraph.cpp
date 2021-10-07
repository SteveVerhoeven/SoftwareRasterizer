#include "Scenegraph.h"

// Features includes
#include "structs.h"
#include "Camera.h"
#include "TriangleMesh.h"

// Extra includes
#include <vector>
#include <iostream>

// Elite includes
#include "ETimer.h"

// SDL includes
#include "SDL.h"

Scenegraph::Scenegraph()
		  : m_pCamera{ nullptr }
		  , m_ScreenSize{}
{}
Scenegraph::~Scenegraph()
{
	DELETEPOINTER(m_pCamera);
	DELETEPOINTERS(m_Meshes, m_Meshes.size());

	m_DepthBuffer.clear();
}

void Scenegraph::Initialize(const uint32_t screenWidth, const uint32_t screenHeight, const Elite::FPoint3& camPos)
{
	// Initialize variables for future reference
	m_pCamera = new Camera(Elite::FPoint3(camPos), 60);
	m_pCamera->Initialize(screenWidth, screenHeight);

	m_ScreenSize.width = screenWidth;
	m_ScreenSize.height = screenHeight;

	int size{ int(screenWidth * screenHeight) };
	for (int i = 0; i < size; ++i)
	{
		m_DepthBuffer.push_back(FLT_MAX);
	}
}
void Scenegraph::Update(const float elapsedSec, const bool UpdateMesh)
{
	if (!UpdateMesh)
		return;

	m_pCamera->Update(elapsedSec);
	for (size_t i{}; i < m_Meshes.size(); ++i)
	{
		m_Meshes[i]->Update(elapsedSec);
	}
}

/* ************************************************ */
/* WIP: Switch from triangle list to triangle strip */
/* ************************************************ */
void Scenegraph::Tag_RemoveDupplicates()
{
	//bool hasDuplicates{ false };
	//size_t iBufferSize{ m_IBuffer.size() };
	//for (size_t i{ 2 }; i < iBufferSize - 1; i += 1)
	//{
	//	auto val1{ m_IBuffer[(i + 0) - 2] };	// Current index
	//	auto val2Current{ m_IBuffer[i] };
	//	auto val3{ m_IBuffer[(i + 1) - 2] };	// Next index
	//	auto val4Current{ m_IBuffer[i + 1] };

	//	if (m_IBuffer[(i + 0) - 2] == m_IBuffer[i] && m_IBuffer[(i + 1) - 2] == m_IBuffer[i + 1])
	//	{
	//		m_IBuffer[i] = -1;
	//		m_IBuffer[i + 1] = -1;

	//		hasDuplicates = true;
	//	}
	//}
	//m_IBuffer.erase(std::remove(m_IBuffer.begin(), m_IBuffer.end(), -1), m_IBuffer.end());

	//if (hasDuplicates)
	//	Tag_RemoveDupplicates();
}
void Scenegraph::FixIBuffer()
{
	/*auto size{ m_IBuffer.size() };
	if (m_IBuffer.size() % 2 == 0)
	{
		auto v1{ m_IBuffer[m_IBuffer.size() / 2 - 1]  };
		auto v2{ m_IBuffer[m_IBuffer.size() / 2] };

		m_IBuffer.insert(m_IBuffer.begin() + (size / 2) - 1, v1);
		m_IBuffer.insert(m_IBuffer.begin() + (size / 2) + 1, v2);
	}*/
}