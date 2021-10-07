//External includes
#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "ERenderer.h"
#include "ERGBColor.h"

#include "Scenegraph.h"
#include "Texture.h"
#include "TriangleMesh.h"
#include "structs.h"

#include <future>

#include "Triangle.h"

#define SINGLE_THREAD
//#define MULTI_THREAD

Elite::Renderer::Renderer(SDL_Window * pWindow)
	 : m_Renderstate{ true }
{
	//Initialize
	m_pWindow = pWindow;
	m_pFrontBuffer = SDL_GetWindowSurface(pWindow);
	int width, height = 0;
	SDL_GetWindowSize(pWindow, &width, &height);
	m_Width = static_cast<uint32_t>(width);
	m_Height = static_cast<uint32_t>(height);
	m_pBackBuffer = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0);
	m_pBackBufferPixels = (uint32_t*)m_pBackBuffer->pixels;
}

void Elite::Renderer::Render(Scenegraph* pScenegraph, const TriangleMesh& tMesh, const bool useTextures, const bool useDiffuseTextureOnly, const bool useAllTextures) const
{
	SDL_LockSurface(m_pBackBuffer);

	std::vector<size_t> iBuffer{ (tMesh.GetTopology() == PrimitiveTopology::TRIANGLE_LIST) ? tMesh.GetIBufferList() : tMesh.GetIBufferStrip() };
	std::vector<Vertex> vBuffer{ tMesh.GetVBuffer() };

	#ifdef SINGLE_THREAD
	for (uint32_t r{0}; r < m_Height; ++r)
	{
		for (uint32_t c{0}; c < m_Width; ++c)
		{
			const int index{ int(c + (r * m_Width)) };
			pScenegraph->SetDepthBufferAtIndex(index, FLT_MAX);
			m_pBackBufferPixels[index] = SDL_MapRGB(m_pBackBuffer->format,
				static_cast<uint8_t>(112),
				static_cast<uint8_t>(126),
				static_cast<uint8_t>(144));
		}
	}

	size_t iBufferSize{ iBuffer.size() };
	Triangle triangle{};

	size_t iBufferMod{ (tMesh.GetTopology() == PrimitiveTopology::TRIANGLE_LIST) ? (size_t)3 : (size_t)1 };
	size_t iBufferSizeMod{ (tMesh.GetTopology() == PrimitiveTopology::TRIANGLE_LIST) ? (size_t)2 : (size_t)2 };

	//for (size_t i{}; i < iBufferSize - 2; i += 3)
	for (size_t i{}; i < iBufferSize - iBufferSizeMod; i += iBufferMod)
	{
		// Creating a triangle
		Vertex v0{};
		Vertex v1{};
		Vertex v2{};

		if (tMesh.GetTopology() == PrimitiveTopology::TRIANGLE_LIST)
		{
			v0 = vBuffer[iBuffer[(size_t)i + 0]];
			v1 = vBuffer[iBuffer[(size_t)i + 1]];
			v2 = vBuffer[iBuffer[(size_t)i + 2]];
		}
		else
		{
			if (i % 2 == 0)
			{
				v0 = vBuffer[iBuffer[(size_t)i + 0]];
				v1 = vBuffer[iBuffer[(size_t)i + 1]];
				v2 = vBuffer[iBuffer[(size_t)i + 2]];
			}
			else
			{
				v0 = vBuffer[iBuffer[(size_t)i + 0]];
				v1 = vBuffer[iBuffer[(size_t)i + 2]];
				v2 = vBuffer[iBuffer[(size_t)i + 1]];
			}
		}

		triangle.Initialize(v0, v1, v2, m_Width, m_Height, pScenegraph->GetCamera());

		BoundingBox bBox{ triangle.GetTriangleBoundingBox() };
		
		// Looping over all the pixels
		for (uint32_t r = uint32_t(bBox.Smallest.y); r < bBox.Largest.y; ++r)
		{
			for (uint32_t c = uint32_t(bBox.Smallest.x); c < bBox.Largest.x; ++c)
			{
				// Taking a pixel
				const Elite::FVector2 pixel{ float(c), float(r) };
				float depth{};
				if (triangle.Hit(pixel, depth))
				{
					const int index{ int(c + (r * m_Width)) };
					if (depth > 0 && depth < 1 && depth < pScenegraph->GetDepthValueAtIndex(index))
					{
						pScenegraph->SetDepthBufferAtIndex(index, depth);

						Elite::RGBColor finalColor;
						if (m_Renderstate.useDepthBuffer)
						{
							float remap{ Elite::Clamp(Elite::Remap(depth, 0.985f, 1.f), 0.f, 1.f) };
							finalColor = RGBColor{ remap, remap, remap };
						}
						else
						{
							//const TriangleMesh* tMesh{ pScenegraph->GetMeshAtIndex(0) };
							TextureSet tSet{};

							tSet.pDiffuseTexture  = tMesh.GetTexture(Texture::TextureType::DIFFUSE);
							tSet.pNormalTexture   = tMesh.GetTexture(Texture::TextureType::NORMAL);
							tSet.pGlossTexture    = tMesh.GetTexture(Texture::TextureType::GLOSS);
							tSet.pSpecularTexture = tMesh.GetTexture(Texture::TextureType::SPECULAR);

							finalColor = triangle.PixelShading(pixel, useTextures, useDiffuseTextureOnly, useAllTextures, tSet, pScenegraph->GetCamera());
						}

						finalColor.MaxToOne();

						m_pBackBufferPixels[index] = SDL_MapRGB(m_pBackBuffer->format,
							static_cast<uint8_t>(finalColor.r * 255),
							static_cast<uint8_t>(finalColor.g * 255),
							static_cast<uint8_t>(finalColor.b * 255));
					}
				}
			}
		}
	}
	#endif // SINGLE_THREAD

	#ifdef MULTI_THREAD
	for (uint32_t r{ 0 }; r < m_Height; ++r)
	{
		for (uint32_t c{ 0 }; c < m_Width; ++c)
		{
			const int index{ int(c + (r * m_Width)) };
			pScenegraph->SetDepthBufferAtIndex(index, FLT_MAX);
			//m_DepthBuffer[(size_t)index] = FLT_MAX;
			m_pBackBufferPixels[index] = SDL_MapRGB(m_pBackBuffer->format,
				static_cast<uint8_t>(112),
				static_cast<uint8_t>(126),
				static_cast<uint8_t>(144));
		}
	}

	//std::size_t cores = std::thread::hardware_concurrency();
	std::size_t cores = 2;
	std::vector<std::future<void>> future_vector;
	size_t indicesSize{ iBuffer.size() };
	uint32_t maxIndicesEachCore = (uint32_t)std::ceil(uint32_t(indicesSize) / (uint32_t)cores);
	if (indicesSize < cores)
	{
		cores = indicesSize;
	}
	for (std::size_t i{}; i < cores; ++i)
	{
		future_vector.emplace_back(std::async([i, &pScenegraph, cores, iBuffer, vBuffer, this, maxIndicesEachCore]()
		{
			size_t indexForThisCore{ i * maxIndicesEachCore };
			size_t maxIndicesForThisCore{ (i + 1) * maxIndicesEachCore };

			// Loop over all triangles
			for (size_t i{ indexForThisCore }; i < maxIndicesForThisCore && i < (maxIndicesEachCore * cores) - 2; i += 3)
			{
				Vertex v0{ vBuffer[iBuffer[i + 0]] };
				Vertex v1{ vBuffer[iBuffer[i + 1]] };
				Vertex v2{ vBuffer[iBuffer[i + 2]] };

				Triangle triangle{ v0, v1, v2, m_Width, m_Height, pScenegraph->GetCamera() };
				BoundingBox triangleBbox{ triangle.GetTriangleBoundingBox() };
				// Loop over all pixels
				for (uint32_t r{ uint32_t(triangleBbox.Smallest.y) }; r < triangleBbox.Largest.y; ++r)
				{
					for (uint32_t c{ uint32_t(triangleBbox.Smallest.x) }; c < triangleBbox.Largest.x; ++c)
					{
						const Elite::FVector2 point{ float(c), float(r) };
						float depth{};
						if (triangle.Hit(point, depth))
						{
							const int index{ int(c + (r * m_Width)) };
							if (depth > 0 && depth < 1 && depth < pScenegraph->GetDepthValueAtIndex(index))
							//if (depth > 0 && depth < 1 && depth < m_DepthBuffer[index])
							{
								pScenegraph->SetDepthBufferAtIndex(index, depth);
								//m_DepthBuffer[index] = depth;

								Elite::RGBColor finalColor;
								if (m_Renderstate.useDepthBuffer)
								{
									float remap{ Elite::Remap(depth, 0.985f, 1.f) };
									finalColor = RGBColor{ remap, remap, remap };
								}
								else
								{
									const TriangleMesh* tMesh{ pScenegraph->GetMeshAtIndex(0) };

									Texture* pDiffuseTexture{ tMesh->GetTexture(Texture::TextureType::DIFFUSE) };
									Texture* pNormalTexture{ tMesh->GetTexture(Texture::TextureType::NORMAL) };
									Texture* pGlossTexture{ tMesh->GetTexture(Texture::TextureType::GLOSS) };
									Texture* pSpecularTexture{ tMesh->GetTexture(Texture::TextureType::SPECULAR) };

									finalColor = triangle.PixelShading(point, pDiffuseTexture, pNormalTexture, pGlossTexture, pSpecularTexture, pScenegraph->GetCamera());
								}

								if (finalColor.r > 1.f ||
									finalColor.g > 1.f ||
									finalColor.b > 1.f)
									finalColor.MaxToOne();

								m_pBackBufferPixels[index] = SDL_MapRGB(m_pBackBuffer->format,
									static_cast<uint8_t>(finalColor.r * 255),
									static_cast<uint8_t>(finalColor.g * 255),
									static_cast<uint8_t>(finalColor.b * 255));
							}
						}
					}
				}
			}
		}));
	}
	for (size_t i{}; i < future_vector.size(); ++i)
	{
		future_vector[i].wait();
	}
	#endif // MULTI_THREAD - WIP
	
	SDL_UnlockSurface(m_pBackBuffer);
	SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
	SDL_UpdateWindowSurface(m_pWindow);
}

bool Elite::Renderer::SaveBackbufferToImage() const
{
	return SDL_SaveBMP(m_pBackBuffer, "BackbufferRender.bmp");
}