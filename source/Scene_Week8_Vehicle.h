#pragma once
#include "Scene.h"

class Scene_Week8_Vehicle final : public Scene
{
	public:
		Scene_Week8_Vehicle() = default;
		~Scene_Week8_Vehicle() = default;

		Scene_Week8_Vehicle(const Scene_Week8_Vehicle&) = delete;
		Scene_Week8_Vehicle(Scene_Week8_Vehicle&&) noexcept = delete;
		Scene_Week8_Vehicle& operator=(const Scene_Week8_Vehicle&) = delete;
		Scene_Week8_Vehicle& operator=(Scene_Week8_Vehicle&&) noexcept = delete;

		virtual void Initialize(const uint32_t screenWidth, const uint32_t screenHeight, SDL_Window* pWindow, const Elite::FPoint3& camPos) override;
		virtual void Update(const float elapsedSec) override;
		virtual void Render() const override;

	private:
		virtual void CreateScene() override;
};