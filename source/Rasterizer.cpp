#include "Rasterizer.h"

// General includes
#include <vector>
#include <iostream>

// External includes
#include "SDL.h"

// Features includes
#include "Scene_Week6.h"
#include "Scene_Week7.h"
#include "Scene_Week7_TukTuk.h"
#include "Scene_Week8_Vehicle.h"
#include "InputManager.h"
#include "structs.h"					 // DELETEPOINTER(X) - DELETEPOINTERS(X)

/* ***************************************** */
/* (un)comment weeks to get the correct week */
/* ***************************************** */
#define Week6_Depth
#define Week7_Texture_Meshes
#define Week8_Shading
/* ***************************************** */
/* (un)comment weeks to get the correct week */
/* ***************************************** */

Rasterizer::Rasterizer()
		  : m_ActiveSceneIndex{}
		  , m_pInputManager{ nullptr }
		  , m_pScenes{ std::vector<Scene*>() }
{}
Rasterizer::~Rasterizer()
{
	DELETEPOINTER(m_pInputManager);
	DELETEPOINTERS(m_pScenes, m_pScenes.size());
}

void Rasterizer::Initialize(SDL_Window* pWindow, const uint32_t screenWidth, const uint32_t screenHeight)
{
	m_ActiveSceneIndex = 0;
	m_pInputManager = new InputManager();
	m_pInputManager->Initialize();
	
	#ifdef Week6_Depth
	Scene_Week6* pScene_W6{ new Scene_Week6 };
	pScene_W6->Initialize(screenWidth, screenHeight, pWindow, Elite::FPoint3(0, 0, 10));
	m_pScenes.push_back(pScene_W6);
	#endif // Week6_Depth
	
	#ifdef Week7_Texture_Meshes
	Scene_Week7* pScene_W7{ new Scene_Week7 };
	pScene_W7->Initialize(screenWidth, screenHeight, pWindow, Elite::FPoint3(0, 0, 10));
	m_pScenes.push_back(pScene_W7);

	Scene_Week7_TukTuk* pScene_W7_TukTuk{ new Scene_Week7_TukTuk };
	pScene_W7_TukTuk->Initialize(screenWidth, screenHeight, pWindow, Elite::FPoint3(0, 5, 30));
	m_pScenes.push_back(pScene_W7_TukTuk);
	#endif // Week7_Texture

	#ifdef Week8_Shading
	Scene_Week8_Vehicle* pScene_W8_Vehicle{ new Scene_Week8_Vehicle };
	pScene_W8_Vehicle->Initialize(screenWidth, screenHeight, pWindow, Elite::FPoint3(0, 5, 65));
	m_pScenes.push_back(pScene_W8_Vehicle);
	#endif // Week8_Shading	
	
	OutputKeyInputs();
}

void Rasterizer::Update(const float elapsedSec, bool& isLooping, bool& takeScreenshot)
{
	m_ActiveSceneIndex = m_pInputManager->Update(elapsedSec, m_pScenes[m_ActiveSceneIndex]->GetSceneGraph(), m_pScenes[m_ActiveSceneIndex]->GetRenderer(), m_ActiveSceneIndex, isLooping, takeScreenshot, int(m_pScenes.size() - 1));
	m_pScenes[m_ActiveSceneIndex]->Update(elapsedSec);
}

void Rasterizer::Render()
{
	m_pScenes[m_ActiveSceneIndex]->Render();
}

void Rasterizer::OutputKeyInputs()
{
	std::cout << "+<==========================================>+\n";
	std::cout << "|<------------------------------------------>|\n";
	std::cout << "|| Rasterizer: Steve Verhoeven - 2DAE02     ||\n";
	std::cout << "|<------------------------------------------>|\n";
	std::cout << "|<------------------------------------------>|\n";
	std::cout << "||                Input keys                ||\n";
	std::cout << "|<------------------------------------------>|\n";
	std::cout << "|| Take screenshot: X                       ||\n";
	std::cout << "|<------------------------------------------>|\n";
	std::cout << "|| Change scene: S                          ||\n";
	std::cout << "|<------------------------------------------>|\n";
	std::cout << "|< Change topology (Quad scene): T          >|\n";
	std::cout << "|<------------------------------------------>|\n";
	std::cout << "|< Change depth only: R                     >|\n";
	std::cout << "|<------------------------------------------>|\n";
	std::cout << "|<------------------------------------------>|\n";
	std::cout << "+<==========================================>+\n";
}