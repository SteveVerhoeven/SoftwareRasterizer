#pragma once
#include <vector>

class Scene;
struct SDL_Window;
class InputManager;
namespace Elite
{
	class Timer;
}
class Rasterizer final
{
	public:
		Rasterizer();
		~Rasterizer();

		Rasterizer(const Rasterizer&) = delete;
		Rasterizer(Rasterizer&&) noexcept = delete;
		Rasterizer& operator=(const Rasterizer&) = delete;
		Rasterizer& operator=(Rasterizer&&) noexcept = delete;

		void Initialize(SDL_Window* pWindow, const uint32_t screenWidth, const uint32_t screenHeight);
		void Update(const float elapsedSec, bool& isLooping, bool& takeScreenshot);
		void Render();

		Scene* GetCurrentScene() const { return m_pScenes[m_ActiveSceneIndex]; }

	protected:
	private:
		int m_ActiveSceneIndex;
		InputManager* m_pInputManager;
		std::vector<Scene*> m_pScenes;

		void OutputKeyInputs();
};