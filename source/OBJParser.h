#pragma once
#include <string>
#include <vector>

struct Vertex;
struct VertexCheck;
class Camera;
class OBJParser final
{
	public:
		OBJParser() = default;
		~OBJParser() = default;

		OBJParser(const OBJParser&) = delete;
		OBJParser(OBJParser&&) noexcept = delete;
		OBJParser& operator=(const OBJParser&) = delete;
		OBJParser& operator=(OBJParser&&) noexcept = delete;

		void LoadModel(const std::string& givenName, const std::string& filePath, std::vector<Vertex>& vBuffer, std::vector<size_t>& iBuffer);
	
	private:
		void checkVertexExists(VertexCheck& vCheck1, VertexCheck& vCheck2, VertexCheck& vCheck3, std::vector<Vertex>& vBuffer);
};