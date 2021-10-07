#pragma once
#include "Scene.h"

class Scene_Week7_TukTuk final : public Scene
{
	public:
		Scene_Week7_TukTuk() = default;
		~Scene_Week7_TukTuk() = default;

		Scene_Week7_TukTuk(const Scene_Week7_TukTuk&) = delete;
		Scene_Week7_TukTuk(Scene_Week7_TukTuk&&) noexcept = delete;
		Scene_Week7_TukTuk& operator=(const Scene_Week7_TukTuk&) = delete;
		Scene_Week7_TukTuk& operator=(Scene_Week7_TukTuk&&) noexcept = delete;

		virtual void Initialize(const uint32_t screenWidth, const uint32_t screenHeight, SDL_Window * pWindow, const Elite::FPoint3& camPos) override;
		virtual void Update(const float elapsedSec) override;
		virtual void Render() const override;

	private:
		virtual void CreateScene() override;
};