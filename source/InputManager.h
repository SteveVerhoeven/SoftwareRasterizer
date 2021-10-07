#pragma once

class Camera;
class Scenegraph;
namespace Elite
{
	class Renderer;
}
class InputManager final
{
	public:
		InputManager() = default;
		~InputManager() = default;

		InputManager(const InputManager&) = delete;
		InputManager(InputManager&&) noexcept = delete;
		InputManager& operator=(const InputManager&) = delete;
		InputManager& operator=(InputManager&&) noexcept = delete;

		void Initialize();
		int Update(const float elapsedSec, Scenegraph* pScenegraph, Elite::Renderer* pRenderer, const int activeSceneIndex, bool& isLooping, bool& takeScreenshot, const int nmrScenes);

	private:
		bool m_DelayTime;
		float m_CurrentDelayTime;
		float m_MaxDelayTime;

		int UpdateScenes(const int activeSceneIndex, const int nmrScenes);
		void UpdateCamera(const float elapsedSec, Camera* pCamera);
		void UpdatePrimitiveTopology(Scenegraph* pScenegraph, const int activeSceneIndex);
		void UpdateShowDepthBuffer(Elite::Renderer* pRenderer);
		void DelayInput(const float elapsedSec);
};