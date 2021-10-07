#pragma once
#pragma warning(disable:4127)

#include <vector>

#include "structs.h"

class Camera;
class Texture;
namespace Elite
{
	struct RGBColor;
}
class Triangle final
{
	public:
		enum class AttributeType
		{
			UV,
			NORMAL,
			TANGENT,
			WORLDPOS
		};

		Triangle() = default;
		~Triangle() = default;

		Triangle(const Triangle&) = delete;
		Triangle(Triangle&&) noexcept = delete;
		Triangle& operator=(const Triangle&) = delete;
		Triangle& operator=(Triangle&&) noexcept = delete;

		void Initialize(const Vertex& v0, const Vertex& v1, const Vertex& v2, const uint32_t screenWidth, const uint32_t screenHeight, const Camera* pCamera);

		bool Hit(const Elite::FVector2& point, float& depth) const;
		Elite::RGBColor PixelShading(const Elite::FVector2& pixel, const bool useTextures, const bool useDiffuseTextureOnly, const bool useAllTextures, const TextureSet& textureSet, Camera* pCamera);

		void UpdateBoundingBox(BoundingBox& bBox, const uint32_t w, const uint32_t h);
		const BoundingBox& GetTriangleBoundingBox() const { return m_Boundingbox; }

	private:
		float m_TotalArea;
		BoundingBox m_Boundingbox;
		std::vector<Vertex> m_VerticesSS, m_VerticesWS;
		Elite::FVector2 m_Vertex0_XY, m_Vertex1_XY, m_Vertex2_XY;	// 2D vertex positions for future calculations
		
		void TransformVertices(const Camera* pCamera, const uint32_t screenWidth, const uint32_t screenHeight);
		bool InsideSignedAreaPara(const Elite::FVector2& v0, const Elite::FVector2& v1, const Elite::FVector2& v2, const Elite::FVector2& p) const;
		float InsideSignedAreaTriangle(const Elite::FVector2& a, const Elite::FVector2& b, const Elite::FVector2& p) const;

		Elite::FVector3 CalculateNormal(Texture* pNormalTexture, const NormalVariables& normalVars) const;
		Elite::RGBColor CalculatePhong(Camera* pCamera, const PhongVariables& weightsPixel, Texture* pGlossTexture, Texture* pSpecularTexture) const;

		template<Triangle::AttributeType AT, typename T>
		T Interpolate(const T& a0, const T& a1, const T& a2, const float w0, const float w1, const float w2, const float depthW) const
		{
			T attribute0{ (a0 / m_VerticesSS[0].position.w) * w0 };
			T attribute1{ (a1 / m_VerticesSS[1].position.w) * w1 };
			T attribute2{ (a2 / m_VerticesSS[2].position.w) * w2 };

			if (AT == AttributeType::NORMAL || AT == AttributeType::TANGENT)
				return { Elite::GetNormalized((attribute0 + attribute1 + attribute2) * depthW) };
			else
				return { (attribute0 + attribute1 + attribute2) * depthW };
		}
};