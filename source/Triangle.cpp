#include "Triangle.h"

// Elite includes
#include "EMath.h"
#include "ERGBColor.h"

// Features includes
#include "structs.h"
#include "Camera.h"
#include "Texture.h"

void Triangle::Initialize(const Vertex& v0, const Vertex& v1, const Vertex& v2, const uint32_t screenWidth, const uint32_t screenHeight, const Camera* pCamera)
{
	m_VerticesWS = { v0, v1, v2 };
	m_VerticesSS = { Vertex(), Vertex(), Vertex() };

	// Transform vertices from WorldSpace to ScreenSpace
	TransformVertices(pCamera, screenWidth, screenHeight);
	// Update the bounding box after the transformation of the vertices to have the bounding box in screen space
	UpdateBoundingBox(m_Boundingbox, screenWidth, screenHeight);

	// Create 2D variables from the vertices for later use
	m_Vertex0_XY = { m_VerticesSS[0].position.x, m_VerticesSS[0].position.y };
	m_Vertex1_XY = { m_VerticesSS[1].position.x, m_VerticesSS[1].position.y };
	m_Vertex2_XY = { m_VerticesSS[2].position.x, m_VerticesSS[2].position.y };

	// Calculate the total area of the triangle for later use
	m_TotalArea = InsideSignedAreaTriangle(m_Vertex0_XY, m_Vertex1_XY, m_Vertex2_XY);
}

bool Triangle::Hit(const Elite::FVector2& pixel, float& depth) const
{
	bool isInside{ InsideSignedAreaPara(m_Vertex0_XY, m_Vertex1_XY, m_Vertex2_XY, pixel) };
	if (isInside)
	{
		float w0{ InsideSignedAreaTriangle(m_Vertex1_XY, m_Vertex2_XY, pixel) / m_TotalArea };
		float w1{ InsideSignedAreaTriangle(m_Vertex2_XY, m_Vertex0_XY, pixel) / m_TotalArea };
		float w2{ InsideSignedAreaTriangle(m_Vertex0_XY, m_Vertex1_XY, pixel) / m_TotalArea };
		 
		depth = 1 / (((1 / m_VerticesSS[0].position.z) * w0) +
					 ((1 / m_VerticesSS[1].position.z) * w1) +
					 ((1 / m_VerticesSS[2].position.z) * w2));
	}
	return isInside;
}
Elite::RGBColor Triangle::PixelShading(const Elite::FVector2& pixel, const bool useTextures, const bool useDiffuseTextureOnly, const bool useAllTextures, const TextureSet& textureSet, Camera* pCamera)
{
	Elite::RGBColor color_Black{ 0, 0, 0 };

	/* Light variables */
	const Elite::RGBColor lightColor{ 1, 1, 1 };
	const float lightIntensity{ 5.f };
	const Elite::FVector3 lightDirection{ 0.577f, -0.577f, -0.577f };

	/* Calculating weight for that pixel */
	float w0{ InsideSignedAreaTriangle(m_Vertex1_XY, m_Vertex2_XY, pixel) / m_TotalArea };
	float w1{ InsideSignedAreaTriangle(m_Vertex2_XY, m_Vertex0_XY, pixel) / m_TotalArea };
	float w2{ InsideSignedAreaTriangle(m_Vertex0_XY, m_Vertex1_XY, pixel) / m_TotalArea };

	/* Calculate depth */
	float depthW{ 1 / ((1 / m_VerticesSS[0].position.w) * w0 +
					   (1 / m_VerticesSS[1].position.w) * w1 +
					   (1 / m_VerticesSS[2].position.w) * w2) };

	// Scene with 2 triangle to show the depth testing working
	if (!useTextures)
	{
		return m_VerticesSS[0].color * w0 + 
			   m_VerticesSS[1].color * w1 + 
			   m_VerticesSS[2].color * w2;
	}

	/* Interpolation of attributes */
	const Elite::FVector2 uv_Interpolate{ Interpolate<AttributeType::UV>(m_VerticesSS[0].UV, m_VerticesSS[1].UV, m_VerticesSS[2].UV, w0, w1, w2, depthW) };												// Interpolated UV: (uvP1 + uvP2 + uvP3) * depth(W: the stay in camera space) 
	const Elite::FVector3 normal_Interpolate{ Interpolate<AttributeType::NORMAL>(m_VerticesSS[0].normal, m_VerticesSS[1].normal, m_VerticesSS[2].normal, w0, w1, w2, depthW) };
	
	/* Sample: Values are in the [0, 255.f] range so divide by 255 to make them [0, 1] range*/
	const Elite::RGBColor diffuseMapColor{ textureSet.pDiffuseTexture->Sample(uv_Interpolate) };

	// Scene with Quad and Checker pattern diffuseTexture
	if (useTextures && useDiffuseTextureOnly)
		return diffuseMapColor;

	if (useTextures && useAllTextures)
	{
		/* New normal */
		NormalVariables normalVars{ w0, w1, w2, depthW, uv_Interpolate, normal_Interpolate };
		Elite::FVector3 sampleNormal{ CalculateNormal(textureSet.pNormalTexture, normalVars) };
		/* Phong */
		PhongVariables phongVars{ w0, w1, w2, depthW, uv_Interpolate, normal_Interpolate, lightDirection };
		Elite::RGBColor phongColor{ CalculatePhong(pCamera, phongVars, textureSet.pGlossTexture, textureSet.pSpecularTexture) };

		/* Lambert cosine law */
		const float lambertCosLaw{ (Elite::Dot(-sampleNormal, lightDirection)) };
		if (lambertCosLaw > 0.f)
			return (lightColor * lightIntensity * (diffuseMapColor / float(M_PI)) * lambertCosLaw) + phongColor;
		else
			return color_Black;
	}

	return color_Black;
}

void Triangle::TransformVertices(const Camera* pCamera, const uint32_t screenWidth, const uint32_t screenHeight)
{
	std::vector<Vertex> projectionSpace{ 3 };

	const Elite::FMatrix4 WVPmatrix = pCamera->GetWVPMatrix();
	const Elite::FMatrix4 Wmatrix = pCamera->GetWorldMatrix();

	for (size_t i{}; i < 3; ++i)
	{
		// Projection space
		projectionSpace[i].position = WVPmatrix * m_VerticesWS[i].position;
		m_VerticesWS[i].normal = Elite::FVector3(Wmatrix * Elite::FVector4(m_VerticesWS[i].normal, 0));
		m_VerticesWS[i].tangent = Elite::FVector3(Wmatrix * Elite::FVector4(m_VerticesWS[i].tangent, 0));
		// NDC - Perspective divide
		projectionSpace[i].position.x /= projectionSpace[i].position.w;
		projectionSpace[i].position.y /= projectionSpace[i].position.w;
		projectionSpace[i].position.z /= projectionSpace[i].position.w;
		// Screen space
		m_VerticesSS[i].position.x = (projectionSpace[i].position.x + 1) / 2 * screenWidth;
		m_VerticesSS[i].position.y = (1 - projectionSpace[i].position.y) / 2 * screenHeight;
		m_VerticesSS[i].position.z = projectionSpace[i].position.z;
		m_VerticesSS[i].position.w = projectionSpace[i].position.w;
		// Copy attributes
		m_VerticesSS[i].UV = m_VerticesWS[i].UV;
		m_VerticesSS[i].color = m_VerticesWS[i].color;
		m_VerticesSS[i].normal = m_VerticesWS[i].normal;
		m_VerticesSS[i].tangent = m_VerticesWS[i].tangent;
	}

	projectionSpace.clear();
}
void Triangle::UpdateBoundingBox(BoundingBox& bBox, const uint32_t w, const uint32_t h)
{
	bBox.Smallest = Elite::FPoint2{ float(w), float(h) };
	bBox.Largest = Elite::FPoint2{ 0, 0 };
	for (size_t i{}; i < 3; ++i)
	{
		bBox.Largest.x = std::max(m_VerticesSS[i].position.x, bBox.Largest.x);
		bBox.Largest.y = std::max(m_VerticesSS[i].position.y, bBox.Largest.y);
		bBox.Smallest.x = std::min(m_VerticesSS[i].position.x, bBox.Smallest.x);
		bBox.Smallest.y = std::min(m_VerticesSS[i].position.y, bBox.Smallest.y);
	}

	if (bBox.Smallest.y < 0)
		bBox.Smallest.y = 0;
	if (bBox.Smallest.x < 0)
		bBox.Smallest.x = 0;
	if (bBox.Largest.y > h)
		bBox.Largest.y = float(h);
	if (bBox.Largest.x > w)
		bBox.Largest.x = float(w);
}

bool Triangle::InsideSignedAreaPara(const Elite::FVector2& v0, const Elite::FVector2& v1, const Elite::FVector2& v2, const Elite::FVector2& p) const
{
	if (Elite::Cross(p - v2, v0 - v2) >= 0)
		if (Elite::Cross(p - v1, v2 - v1) >= 0)
			if (Elite::Cross(p - v0, v1 - v0) >= 0)
				return true;

	return false;
}
float Triangle::InsideSignedAreaTriangle(const Elite::FVector2& a, const Elite::FVector2& b, const Elite::FVector2& p) const
{
	// >(Cross(p - a, b - a))/2< is the calculation to get the signed area of a triangle
	return (Elite::Cross(p - a, b - a) / 2);
}

Elite::FVector3 Triangle::CalculateNormal(Texture* pNormalTexture, const NormalVariables& normalVars) const
{
	const Elite::FVector3 tangentInterpolate{ Interpolate<AttributeType::TANGENT>(m_VerticesSS[0].tangent, m_VerticesSS[1].tangent, m_VerticesSS[2].tangent, normalVars.w0, normalVars.w1, normalVars.w2, normalVars.depthW) };

	/* TangentMatrix formation */
	Elite::FVector3 biNormal{ Elite::GetNormalized(Elite::Cross(tangentInterpolate, normalVars.normalInterpolate)) };
	Elite::FMatrix3 tangentSpaceAxis{ Elite::FMatrix3(tangentInterpolate, biNormal, normalVars.normalInterpolate) };

	// Sample
	const Elite::RGBColor normalMapColor{ pNormalTexture->Sample(normalVars.uvInterpolate) };
	
	/* Remap normal sample from [0, 1] to [-1, 1] */
	float x{ 2 * (normalMapColor.r) - 1.f };
	float y{ 2 * (normalMapColor.g) - 1.f };
	float z{ 2 * (normalMapColor.b) - 1.f };
	Elite::FVector3 sampleNormal{ Elite::GetNormalized(tangentSpaceAxis * Elite::FVector3{ x, y, z }) };

	return sampleNormal;
}
Elite::RGBColor Triangle::CalculatePhong(Camera* pCamera, const PhongVariables& phongPixel, Texture* pGlossTexture, Texture* pSpecularTexture) const
{
	const float shininess{ 25.f };

	/* Interpolate WORLDPOS of every vertex */			
	Elite::FVector4 worldPosV0 = Elite::FVector4(pCamera->GetWorldMatrix() * m_VerticesWS[0].position);
	Elite::FVector4 worldPosV1 = Elite::FVector4(pCamera->GetWorldMatrix() * m_VerticesWS[1].position);
	Elite::FVector4 worldPosV2 = Elite::FVector4(pCamera->GetWorldMatrix() * m_VerticesWS[2].position);
	const Elite::FVector3 worldPos_Interpolate{ Interpolate<AttributeType::WORLDPOS>(worldPosV0, worldPosV1, worldPosV2, phongPixel.w0, phongPixel.w1, phongPixel.w2, phongPixel.depthW) };												// Interpolated UV: (uvP1 + uvP2 + uvP3) * depth(W: the stay in camera space) 

	const Elite::RGBColor specularMapColor{ pSpecularTexture->Sample(phongPixel.uvInterpolate) };
	const Elite::RGBColor glossMapColor{ pGlossTexture->Sample(phongPixel.uvInterpolate) };		// Phong exponent

	/* Phong */
	const Elite::FVector3 inverseViewDir{ pCamera->GetInverseViewDirection(Elite::FPoint3(worldPos_Interpolate)) };
	const float exponent{ glossMapColor.r * shininess };

	Elite::FVector3 reflect = (-phongPixel.lightDirection + 2 * (Elite::Dot(phongPixel.normalInterpolate, phongPixel.lightDirection)) * phongPixel.normalInterpolate);
	float cos = Elite::Clamp(Elite::Dot(reflect, -inverseViewDir), 0.f, 1.f);
	Elite::RGBColor phongColor{ specularMapColor * powf(cos, exponent) };
	//phongColor.MaxToOne();

	return phongColor;
}