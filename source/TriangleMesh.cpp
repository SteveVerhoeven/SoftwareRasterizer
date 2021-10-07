#include "TriangleMesh.h"

// General includes
#include <string>

// Features includes
#include "OBJParser.h"
#include "Triangle.h"
#include "structs.h"
#include "Camera.h"
#include "Texture.h"

TriangleMesh::TriangleMesh()
			: m_Topology{}
			, m_pDiffuseTexture{ nullptr }
			, m_pNormalTexture{ nullptr }
			, m_pGlossTexture{ nullptr }
			, m_pSpecularTexture{ nullptr }
			, m_VBuffer{ std::vector<Vertex>() }
			, m_IBufferList{ std::vector<size_t>() }
			, m_IBufferStrip{ std::vector<size_t>() }
{}

TriangleMesh::~TriangleMesh()
{
	DELETEPOINTER(m_pDiffuseTexture);
	DELETEPOINTER(m_pNormalTexture);
	DELETEPOINTER(m_pGlossTexture);
	DELETEPOINTER(m_pSpecularTexture);

	m_VBuffer.clear();
	m_IBufferList.clear();
	m_IBufferStrip.clear();
}

void TriangleMesh::InitializeMesh(const std::string& givenName, const std::string& filePath, const PrimitiveTopology& topology)
{
	m_Topology = topology;

	OBJParser parser;
	parser.LoadModel(givenName, filePath, m_VBuffer, m_IBufferList);
}
void TriangleMesh::InitializeTexture(const std::string& path, const Texture::TextureType& textureType)
{
	if (textureType == Texture::TextureType::DIFFUSE)
		m_pDiffuseTexture = new Texture(path);
	else if (textureType == Texture::TextureType::NORMAL)
		m_pNormalTexture = new Texture(path);
	else if (textureType == Texture::TextureType::GLOSS)
		m_pGlossTexture = new Texture(path);
	else if (textureType == Texture::TextureType::SPECULAR)
		m_pSpecularTexture = new Texture(path);
}

void TriangleMesh::InitializeBuffers(const std::vector<size_t>& iBuffer, const std::vector<Vertex>& vBuffer, const PrimitiveTopology& topology)
{
	m_Topology = topology;

	for (size_t i{}; i < iBuffer.size(); ++i)
	{
		m_IBufferList.push_back(iBuffer[i]);
	}

	for (size_t i{}; i < vBuffer.size(); ++i)
	{
		m_VBuffer.push_back(vBuffer[i]);
	}
}

void TriangleMesh::InitializeBuffers(const std::vector<size_t>& iBufferList, const std::vector<size_t>& iBufferStrip, const std::vector<Vertex>& vBuffer, const PrimitiveTopology& topology)
{
	m_Topology = topology;

	for (size_t i{}; i < iBufferList.size(); ++i)
	{
		m_IBufferList.push_back(iBufferList[i]);
	}
	for (size_t i{}; i < iBufferStrip.size(); ++i)
	{
		m_IBufferStrip.push_back(iBufferStrip[i]);
	}

	for (size_t i{}; i < vBuffer.size(); ++i)
	{
		m_VBuffer.push_back(vBuffer[i]);
	}
}

void TriangleMesh::Update(const float /*elapsedSec*/)
{}