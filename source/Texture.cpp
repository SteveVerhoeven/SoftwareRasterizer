#include "Texture.h"

// General includes
#include <string>

// Extern includes
#include <SDL_image.h>

// Elite includes
#include "ERGBColor.h"

Texture::Texture(const std::string& path)
	   : m_pTexture{ IMG_Load(path.c_str()) }
	   , m_pImagePixels{ (Uint32*)m_pTexture->pixels }
	   , m_pImageFormat{ m_pTexture->format }
{}
Texture::~Texture()
{
	SDL_FreeSurface(m_pTexture);
}

Elite::RGBColor Texture::Sample(const Elite::FVector2& uv)
{
	if (0 < uv.x && uv.x < 1 && 0 < uv.y && uv.y < 1)
	{
		int x = int(uv.x * m_pTexture->w);
		int y = int(uv.y * m_pTexture->h);

		Uint8 r{}, g{}, b{};
		SDL_GetRGB(m_pImagePixels[x + (y * m_pTexture->w)], m_pImageFormat, &r, &g, &b);

		return (Elite::RGBColor{ float(r), float(g), float(b) } / 255.f);
	}
	
	return Elite::RGBColor{ 0.f, 0.f, 0.f };
}
