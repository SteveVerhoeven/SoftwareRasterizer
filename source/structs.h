#pragma once
#include "EMath.h"
#include "ERGBColor.h"
#include "Texture.h"

//using namespace Elite;

#pragma region Macro
/* ***************** */
/* DELETING POINTERS */
/* ***************** */
#define DELETEPOINTER( x ) \
if (x != NULL)      \
{                   \
    delete x;       \
    x = nullptr;       \
}
#define DELETEPOINTERS( x, size ) 	\
for (size_t i{}; i < size; ++i)		\
{									\
	DELETEPOINTER(x[i]);			\
}									\
x.clear();							\
/* *********************** */
/* REMOVING C++17 warnings */
/* *********************** */

#pragma endregion

#pragma region Struct
struct NormalVariables
{
	NormalVariables(const float weight0, const float weight1, const float weight2, const float depth, const Elite::FVector2& uv, const Elite::FVector3& normal)
	{
		w0 = weight0;
		w1 = weight1;
		w2 = weight2;
		depthW = depth;
		normalInterpolate = normal;
		uvInterpolate = uv;
	}

	float w0;
	float w1;
	float w2;
	float depthW;
	Elite::FVector3 normalInterpolate;
	Elite::FVector2 uvInterpolate;
};
struct PhongVariables
{
	PhongVariables(const float weight0, const float weight1, const float weight2, const float depth, const Elite::FVector2& uv, const Elite::FVector3& normal, const Elite::FVector3& lightDir)
	{
		w0 = weight0;
		w1 = weight1;
		w2 = weight2;
		depthW = depth;
		normalInterpolate = normal;
		uvInterpolate = uv;
		lightDirection = lightDir;
	}

	float w0;
	float w1;
	float w2;
	float depthW;
	Elite::FVector3 normalInterpolate;
	Elite::FVector3 lightDirection;
	Elite::FVector2 uvInterpolate;
};
struct TextureSet
{
	Texture* pDiffuseTexture{ nullptr };
	Texture* pNormalTexture{ nullptr };
	Texture* pSpecularTexture{ nullptr };
	Texture* pGlossTexture{ nullptr };
};
struct Ray
{
	Elite::FPoint3 origin{};
	Elite::FVector3 direction{};
	const float tMin{ 0.0001f };
	float tMax{ FLT_MAX };
};
struct HitRecord
{
	bool isHit{};
	float t{};
	float tClosest{ FLT_MAX };
	Elite::FPoint3 point{};
	Elite::FVector3 normal{};
	Elite::RGBColor color{};
	Elite::RGBColor colorToFill{};

	void Reset() 
	{
		isHit = false;
		tClosest = FLT_MAX;
		colorToFill = { 0, 0, 0 };
	}
};

struct Screensize
{
	uint32_t width;
	uint32_t height;
};

struct Vertex
{
	Vertex()
	{ 
		position = Elite::FPoint4(0, 0, 0, 1); 
		color = Elite::RGBColor(0, 0, 0);
		UV = Elite::FVector2(0, 0);
		normal = Elite::FVector3(0, 0, 0);
		tangent = Elite::FVector3(0, 0, 0);
	}
	Vertex(const float pX, const float pY, const float pZ, const float r, const float g, const float b, const float u, const float v, const float nX, const float nY, const float nZ)
	{ 
		position = Elite::FPoint4(pX, pY, pZ, 1);
		color = Elite::RGBColor(r, g, b);
		UV = Elite::FVector2(u, v);
		normal = Elite::FVector3(nX, nY, nZ);
	}
	Vertex(const Elite::FPoint3& p, const Elite::RGBColor& c, const Elite::FVector2& uv, const Elite::FVector3& n)
	{ 
		position = Elite::FPoint4(p, 1);
		color = c;
		UV = uv;
		normal = n;
	}

	Elite::FPoint4 position{};				// To solve Visibility problem
	Elite::FVector2 UV{};					// For texture mapping
	Elite::RGBColor color{};				
	Elite::FVector3 normal{};				// For normal mapping
	Elite::FVector3 tangent{};				// For normal mapping

	bool operator==(const Vertex& v)
	{
		if (v.position == position)
			if (v.normal == normal)
				if (v.UV == UV)
					return true;

		return false;
	}
};
struct VertexCheck
{
	VertexCheck(const Vertex& v)
	{
		found = false;
		foundIndex = -1;
		vertexToCheck = v;
	}
	bool found{};
	int foundIndex{};
	Vertex vertexToCheck;
};

struct RenderState
{
	RenderState()
	{
		useFinalColor = true;
		useDepthBuffer = false;
	}
	RenderState(bool useFinalColor)
	{
		useFinalColor = useFinalColor;
		useDepthBuffer = !useFinalColor;
	}

	bool useFinalColor = true;
	bool useDepthBuffer = false;
};

struct BoundingBox
{
	Elite::FPoint2 Smallest{};
	Elite::FPoint2 Largest{};
};
#pragma endregion

#pragma region Enum
enum class PrimitiveTopology
{
	TRIANGLE_LIST = 0,
	TRIANGLE_STRIP = 1
};
#pragma endregion