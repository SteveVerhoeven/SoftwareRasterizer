#pragma once
#include "Scene.h"

class Scene_Week7 final : public Scene
{
	public:
		Scene_Week7() = default;
		~Scene_Week7() = default;

		Scene_Week7(const Scene_Week7&) = delete;
		Scene_Week7(Scene_Week7&&) noexcept = delete;
		Scene_Week7& operator=(const Scene_Week7&) = delete;
		Scene_Week7& operator=(Scene_Week7&&) noexcept = delete;

		virtual void Initialize(const uint32_t screenWidth, const uint32_t screenHeight, SDL_Window * pWindow, const Elite::FPoint3& camPos) override;
		virtual void Update(const float elapsedSec) override;
		virtual void Render() const override;

	private:
		virtual void CreateScene() override;
};