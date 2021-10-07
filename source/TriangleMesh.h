#pragma once
#include "structs.h"
#include <vector>
#include <string>

class Texture;
class Camera;
class TriangleMesh final
{
	public:
		TriangleMesh();
		~TriangleMesh();

		TriangleMesh(const TriangleMesh&) = delete;
		TriangleMesh(TriangleMesh&&) noexcept = delete;
		TriangleMesh& operator=(const TriangleMesh&) = delete;
		TriangleMesh& operator=(TriangleMesh&&) noexcept = delete;

		void InitializeMesh(const std::string& givenName, const std::string& filePath, const PrimitiveTopology& topology);
		void InitializeTexture(const std::string& path, const Texture::TextureType& textureType);
		void InitializeBuffers(const std::vector<size_t>& iBuffer, const std::vector<Vertex>& vBuffer, const PrimitiveTopology& topology);
		void InitializeBuffers(const std::vector<size_t>& iBufferList, const std::vector<size_t>& iBufferStrip, const std::vector<Vertex>& vBuffer, const PrimitiveTopology& topology);

		void Update(const float elapsedSec);

		const std::vector<size_t>& GetIBufferList() const { return m_IBufferList; }
		const std::vector<size_t>& GetIBufferStrip() const { return m_IBufferStrip; }
		const std::vector<Vertex>& GetVBuffer() const { return m_VBuffer; }
		Texture* GetTexture(const Texture::TextureType& textureType) const 
		{ 
			if (textureType == Texture::TextureType::DIFFUSE && m_pDiffuseTexture)
				return m_pDiffuseTexture;
			else if (textureType == Texture::TextureType::NORMAL && m_pNormalTexture)
				return m_pNormalTexture;
			else if (textureType == Texture::TextureType::GLOSS && m_pGlossTexture)
				return m_pGlossTexture;
			else if (textureType == Texture::TextureType::SPECULAR && m_pSpecularTexture)
				return m_pSpecularTexture;
			
			return nullptr;
		};
		const PrimitiveTopology& GetTopology() const { return m_Topology; }
		void SetTopology(const PrimitiveTopology& topology) { m_Topology = topology; }

	protected:
	private:
		PrimitiveTopology m_Topology;
		Texture* m_pDiffuseTexture;
		Texture* m_pNormalTexture;
		Texture* m_pGlossTexture;
		Texture* m_pSpecularTexture;

		std::vector<Vertex> m_VBuffer;
		std::vector<size_t> m_IBufferList;
		std::vector<size_t> m_IBufferStrip;
};