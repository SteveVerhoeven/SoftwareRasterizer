#include "OBJParser.h"

// General includes
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

// Features includes
#include "structs.h"

void OBJParser::LoadModel(const std::string& givenName, const std::string& filePath, std::vector<Vertex>& vBuffer, std::vector<size_t>& iBuffer)
{
	std::vector<Elite::FPoint3> positions;
	std::vector<Elite::FVector3> normals;
	std::vector<Elite::FVector2> UVs;

	std::string name = givenName;

	std::ifstream in(filePath, std::ios::in);
	if (!in)
	{
		std::cout << ("Model \"" + filePath + "\" does not exist.");
		return;
	}

	std::string line;
	while (getline(in, line))
	{
		if (line.substr(0, 1) == "#" || line.substr(0, 1) == "")
			continue;

		if (line.substr(0, 2) == "v ")
		{
			std::istringstream s{ line.substr(2) };
			Elite::FPoint3 v{};
			s >> v.x >> v.y >> v.z;
			positions.push_back(v);
		}
		else if (line.substr(0, 2) == "vt")
		{
			std::istringstream s(line.substr(2));
			Elite::FVector2 v{};
			s >> v.x >> v.y;
			UVs.push_back(Elite::FVector2(v.x, 1 - v.y));		

			/* Calculation is happening with the Y component on the bottom
			*  So "1 - uv.y" is to convert from OBJ parser uv to the UV used in the rasterizer
			* --------------------------------------------------------------------------------
			*	             UV space rasterizer	   --		UV space OBJ-parser
			*                 [0, 1]---[1, 1]                     [0, 0]---[1, 0]
			*                  ||       ||	                       ||       ||
			*                  ||       ||	                       ||       ||
			*                 [0, 0]---[1, 0]                     [0, 1]---[1, 1]
			* --------------------------------------------------------------------------------
			*/
		}
		else if (line.substr(0, 2) == "vn")
		{
			std::istringstream s(line.substr(2));
			Elite::FVector3 v{};
			s >> v.x >> v.y >> v.z;
			normals.push_back(v);
		}
		else if (line.substr(0, 2) == "f ")
		{
			std::istringstream s(line.substr(1));
			char slash; // To catch the / between the numbers
			unsigned short v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3;
			s >> v1 >> slash >> vt1 >> slash >> vn1;
			s >> v2 >> slash >> vt2 >> slash >> vn2;
			s >> v3 >> slash >> vt3 >> slash >> vn3;

			/**/
			Elite::FPoint3& p0 = positions[--v1];
			Elite::FPoint3& p1 = positions[--v2];
			Elite::FPoint3& p2 = positions[--v3];
			Elite::FVector2& uv0 = UVs[--vt1];
			Elite::FVector2& uv1 = UVs[--vt2];
			Elite::FVector2& uv2 = UVs[--vt3];

			const Elite::FVector3 edge0 = Elite::FVector3(p1 - p0);
			const Elite::FVector3 edge1 = Elite::FVector3(p2 - p0);
			const Elite::FVector2 diffX = Elite::FVector2(uv1.x - uv0.x, uv2.x - uv0.x);
			const Elite::FVector2 diffY = Elite::FVector2(uv1.y - uv0.y, uv2.y - uv0.y);
			float r = 1.f / (Elite::Cross(diffX, diffY));

			Elite::FVector3 tangent = Elite::FVector3((edge0 * diffY.y - edge1 * diffX.x) * r);
			/**/

			Vertex vert0{};
			vert0.position	= Elite::FPoint3{  positions[v1] };
			vert0.UV		= Elite::FVector2{ UVs[vt1] };
			vert0.normal	= Elite::FVector3{ normals[--vn1] };
			vert0.tangent	+= Elite::GetNormalized(Elite::Reject(tangent, normals[vn1]));
			Vertex vert1{};
			vert1.position	= Elite::FPoint3{  positions[v2] };
			vert1.UV		= Elite::FVector2{ UVs[vt2] };
			vert1.normal	= Elite::FVector3{ normals[--vn2] };
			vert1.tangent   += Elite::GetNormalized(Elite::Reject(tangent, normals[vn2]));
			Vertex vert2{};
			vert2.position	= Elite::FPoint3{  positions[v3] };
			vert2.UV		= Elite::FVector2{ UVs[vt3] };
			vert2.normal	= Elite::FVector3{ normals[--vn3] };
			vert2.tangent   += Elite::GetNormalized(Elite::Reject(tangent, normals[vn3]));

			VertexCheck vCheck1{ vert0 };
			VertexCheck vCheck2{ vert1 };
			VertexCheck vCheck3{ vert2 };

			checkVertexExists(vCheck1, vCheck2, vCheck3, vBuffer);
			{
				if (!vCheck1.found)
				{
					vBuffer.push_back(vert0);
					iBuffer.push_back(vBuffer.size() - 1);
				}
				else
				{
					iBuffer.push_back(vCheck1.foundIndex);
				}
				if (!vCheck2.found)
				{
					vBuffer.push_back(vert1);
					iBuffer.push_back(vBuffer.size() - 1);
				}
				else
				{
					iBuffer.push_back(vCheck2.foundIndex);
				}
				if (!vCheck3.found)
				{
					vBuffer.push_back(vert2);
					iBuffer.push_back(vBuffer.size() - 1);
				}
				else
				{
					iBuffer.push_back(vCheck3.foundIndex);
				}
			}
		}
	}

	std::cout << ("Model \"" + name + "\" parsed & loaded in vBuffer, iBuffer. \n");
}

void OBJParser::checkVertexExists(VertexCheck& vCheck0, VertexCheck& vCheck1, VertexCheck& vCheck2, std::vector<Vertex>& vBuffer)
{
	for (size_t i{}; i < vBuffer.size(); ++i)
	{
		if (!vCheck0.found && vBuffer[i] == vCheck0.vertexToCheck)
		{
			vCheck0.found = true;
			vCheck0.foundIndex = (int)i;
		}
		if (!vCheck1.found && vBuffer[i] == vCheck1.vertexToCheck)
		{
			vCheck1.found = true;
			vCheck1.foundIndex = (int)i;
		}
		if (!vCheck2.found && vBuffer[i] == vCheck2.vertexToCheck)
		{
			vCheck2.found = true;
			vCheck2.foundIndex = (int)i;
		}

		if (vCheck0.found && vCheck1.found && vCheck2.found)
			break;
	}
}