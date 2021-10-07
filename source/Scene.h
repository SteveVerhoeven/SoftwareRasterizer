#pragma once
#include <cstdint>
#include "EMath.h"
#include "TriangleMesh.h"

class Scenegraph;
class Camera;
struct SDL_Window;
namespace Elite
{
	class Timer;
	class Renderer;
}
class Scene
{
	public:
		Scene();
		virtual ~Scene();

		Scene(const Scene&) = delete;
		Scene(Scene&&) noexcept = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) noexcept = delete;

		virtual void Initialize(const uint32_t screenWidth, const uint32_t screenHeight, SDL_Window* pWindow, const Elite::FPoint3& camPos);
		virtual void Update(const float elapsedSec);
		virtual void Render() const;

		virtual Scenegraph* GetSceneGraph() const { return m_pSceneGraph; };
		virtual Elite::Renderer* GetRenderer() const { return m_pERenderer; };

	protected:
		Scenegraph* m_pSceneGraph;
		Elite::Renderer* m_pERenderer;		

		bool m_UseTextures;
		bool m_UseDiffuseTextureOnly;
		bool m_UseAllTextures;

		bool m_UpdateMesh;

		virtual void CreateScene() = 0;

	private:

};