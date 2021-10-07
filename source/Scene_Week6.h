#pragma once
#include "Scene.h"

class Scene_Week6 final : public Scene
{
	public:
		Scene_Week6() = default;
		~Scene_Week6() = default;

		Scene_Week6(const Scene_Week6&) = delete;
		Scene_Week6(Scene_Week6&&) noexcept = delete;
		Scene_Week6& operator=(const Scene_Week6&) = delete;
		Scene_Week6& operator=(Scene_Week6&&) noexcept = delete;

		virtual void Initialize(const uint32_t screenWidth, const uint32_t screenHeight, SDL_Window* pWindow, const Elite::FPoint3& camPos) override;
		virtual void Update(const float elapsedSec) override;
		virtual void Render() const override;
			
	private:
		virtual void CreateScene() override;
};