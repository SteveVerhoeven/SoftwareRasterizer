#pragma once
#include <string>

#include "EMath.h"

#include <SDL_image.h>

namespace Elite
{
	struct RGBColor;
}
class Texture final
{
	public:
		enum class TextureType
		{
			DIFFUSE,
			NORMAL,
			GLOSS,
			SPECULAR
		};

		Texture(const std::string& path);
		~Texture();

		Texture(const Texture&) = delete;
		Texture(Texture&&) noexcept = delete;
		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture&&) noexcept = delete;

		Elite::RGBColor Sample(const Elite::FVector2& uv);

	private:
		SDL_Surface* m_pTexture;
		const Uint32* m_pImagePixels;
		const SDL_PixelFormat* m_pImageFormat;
};