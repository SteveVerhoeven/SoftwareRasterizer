#include "InputManager.h"

// External includes
#include "SDL.h"

// Features includes
#include "Camera.h"
#include "Scenegraph.h"
#include "TriangleMesh.h"

// General includes
#include <iostream>

// Elite includes
#include "ERenderer.h"

void InputManager::Initialize()
{
	m_DelayTime = false;
	m_CurrentDelayTime = 0;
	m_MaxDelayTime = 0.5f;
}

int InputManager::Update(const float elapsedSec, Scenegraph* pScenegraph, Elite::Renderer* pRenderer, const int activeSceneIndex, bool& isLooping, bool& takeScreenshot, const int nmrScenes)
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
			case SDL_QUIT:
				isLooping = false;
				break;
			case SDL_KEYUP:
				if (e.key.keysym.scancode == SDL_SCANCODE_X)
					takeScreenshot = true;
				break;
		}
	}

	UpdateCamera(elapsedSec, pScenegraph->GetCamera());
	UpdatePrimitiveTopology(pScenegraph, activeSceneIndex);
	UpdateShowDepthBuffer(pRenderer);

	if (m_DelayTime)
		DelayInput(elapsedSec);

	return UpdateScenes(activeSceneIndex, nmrScenes);
}

void InputManager::UpdateCamera(const float elapsedSec, Camera* pCamera)
{
	int x{}, y{};
	const float inputMultiplier = 20.f;
	const Uint32 mouseState = SDL_GetRelativeMouseState(&x, &y);

	if (SDL_BUTTON(1) == mouseState)		// LB
	{
		pCamera->MoveForward(y * elapsedSec * inputMultiplier, x * elapsedSec * inputMultiplier);
	}
	else if (SDL_BUTTON(3) == mouseState)	// RB
	{
		float xAngle = (float(x) * inputMultiplier * elapsedSec) * float(E_TO_RADIANS);
		float yAngle = (float(y) * inputMultiplier * elapsedSec) * float(E_TO_RADIANS);

		pCamera->MoveRotation(xAngle, yAngle);
	}
	else if ((SDL_BUTTON(1) + SDL_BUTTON(3)) == mouseState)		// LB + RB
	{
		pCamera->MoveUp(float(-y * elapsedSec) * inputMultiplier);
	}
}

void InputManager::UpdatePrimitiveTopology(Scenegraph* pScenegraph, const int activeSceneIndex)
{
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// T: Switch topology
	if (state[SDL_SCANCODE_T] && !m_DelayTime)
	{
		// Quadscene
		if (activeSceneIndex == 1)
		{
			TriangleMesh* pCurrentMesh{ pScenegraph->GetMeshAtIndex(0) };

			if (pCurrentMesh->GetTopology() == PrimitiveTopology::TRIANGLE_LIST)
			{
				pCurrentMesh->SetTopology(PrimitiveTopology::TRIANGLE_STRIP);
				std::cout << "Topology: Switched to Triangle  >strip< \n";
			}
			else
			{
				pCurrentMesh->SetTopology(PrimitiveTopology::TRIANGLE_LIST);
				std::cout << "Topology: Switched to Triangle  >list< \n";
			}
			m_DelayTime = true;
		}		
	}
}

void InputManager::UpdateShowDepthBuffer(Elite::Renderer* pRenderer)
{
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// R: Show final color or depthbuffer
	if (state[SDL_SCANCODE_R] && !m_DelayTime)
	{
		pRenderer->SetRenderState(!pRenderer->GetRenderstate().useFinalColor);
		m_DelayTime = true;
	}
}

void InputManager::DelayInput(const float elapsedSec)
{
	if (m_CurrentDelayTime > m_MaxDelayTime)
	{
		m_CurrentDelayTime = 0;
		m_DelayTime = false;
		//std::cout << "Delay done\n";
	}
	else
		m_CurrentDelayTime += elapsedSec;
}
int InputManager::UpdateScenes(const int activeSceneIndex, const int nmrScenes)
{
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// S: Switch scene
	if (state[SDL_SCANCODE_S] && !m_DelayTime)
	{
		m_DelayTime = true;
		
		int idx{ activeSceneIndex + 1 };
		if (idx > nmrScenes)
			return 0;
		else
			return idx;
	}

	return activeSceneIndex;
}