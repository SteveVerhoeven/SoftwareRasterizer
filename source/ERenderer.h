/*=============================================================================*/
// Copyright 2017-2019 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// ERenderer.h: class that holds the surface to render to, does traverse the pixels 
// and traces the rays using a tracer
/*=============================================================================*/
#ifndef ELITE_RAYTRACING_RENDERER
#define	ELITE_RAYTRACING_RENDERER

#include <cstdint>
#include <vector>

#include "structs.h"

class Camera;
class Scenegraph;
class TriangleMesh;
struct SDL_Window;
struct SDL_Surface;
namespace Elite
{
	class Renderer final
	{
		public:
			Renderer(SDL_Window* pWindow);
			~Renderer() = default;

			Renderer(const Renderer&) = delete;
			Renderer(Renderer&&) noexcept = delete;
			Renderer& operator=(const Renderer&) = delete;
			Renderer& operator=(Renderer&&) noexcept = delete;

			void Render(Scenegraph* pScenegraph, const TriangleMesh& tMesh, const bool useTextures, const bool useDiffuseTextureOnly, const bool useAllTextures) const;
			bool SaveBackbufferToImage() const;

			const RenderState& GetRenderstate() const { return m_Renderstate; }
			void SetRenderState(const bool useFinalcolor) 
			{ 
				m_Renderstate.useFinalColor = useFinalcolor; 
				m_Renderstate.useDepthBuffer = !useFinalcolor; 
			}

		private:
			SDL_Window* m_pWindow = nullptr;
			SDL_Surface* m_pFrontBuffer = nullptr;
			SDL_Surface* m_pBackBuffer = nullptr;
			uint32_t* m_pBackBufferPixels = nullptr;
			uint32_t m_Width = 0;
			uint32_t m_Height = 0;

			RenderState m_Renderstate;
	};
}

#endif